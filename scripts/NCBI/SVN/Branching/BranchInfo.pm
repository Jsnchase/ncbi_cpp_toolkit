# $Id$

package NCBI::SVN::BranchInfo;

use strict;
use warnings;

use base qw(NCBI::SVN::Base);

sub SetUpstreamAndSynchRev
{
    my ($Self, $SourcePath, $BranchDir, $SourceRevision, $Revision) = @_;

    my $UpstreamPath = $SourcePath;

    length($UpstreamPath) > length($BranchDir) and
        substr($UpstreamPath, -length($BranchDir),
            length($BranchDir), '') eq $BranchDir
        or die 'branch structure does not replicate ' .
            "upstream structure in r$Revision->{Number}";

    unless (my $SameUpstreamPath = $Self->{UpstreamPath})
    {
        $Self->{UpstreamPath} = $UpstreamPath
    }
    else
    {
        die "inconsistent source paths in r$Revision->{Number}"
            if $UpstreamPath ne $SameUpstreamPath
    }

    unless (my $SingleSourceRevision = $Self->{LastSynchRevision})
    {
        $Self->{LastSynchRevision} = $SourceRevision
    }
    else
    {
        die "inconsistent source revision numbers in r$Revision->{Number}"
            if $SourceRevision != $SingleSourceRevision
    }
}

sub MergeDeletedTrees
{
    my ($ObsoletePathTree, $Subdir, $DeletedNode) = @_;

    if (exists $DeletedNode->{'/'})
    {
        $ObsoletePathTree->{$Subdir} = {'/' => 1};

        return
    }

    $ObsoletePathTree = ($ObsoletePathTree->{$Subdir} ||= {});

    unless (exists $ObsoletePathTree->{'/'})
    {
        while (my ($Subdir, $Subtree) = each %$DeletedNode)
        {
            MergeDeletedTrees($ObsoletePathTree, $Subdir, $Subtree)
        }
    }

    return 0
}

sub ClearDeletedTree
{
    my ($ObsoletePathTree, $BranchStructure) = @_;

    while (my ($Subdir, $Subtree) = each %$BranchStructure)
    {
        if ($Subtree->{'/'})
        {
            delete $ObsoletePathTree->{$Subdir};

            next
        }

        if (my $ObsoletePathSubtree = $ObsoletePathTree->{$Subdir})
        {
            ClearDeletedTree($ObsoletePathSubtree, $Subtree)
        }
    }

    return 0
}

sub ModelBranchStructure
{
    my ($Self, $BranchStructure, $Revision, $CommonTarget) = @_;

    for my $Change (@{$Revision->{ChangedPaths}})
    {
        my ($ChangeType, $BranchDir, $SourcePath, $SourceRevision) = @$Change;

        next if substr($BranchDir, 0, length($CommonTarget), '')
            ne $CommonTarget;

        if ($ChangeType eq 'D' || $ChangeType eq 'R')
        {
            my $ParentNode = $BranchStructure;

            my @Subdirs = split('/', $BranchDir);
            my $Name = pop(@Subdirs);

            for my $Subdir (@Subdirs)
            {
                $ParentNode = ($ParentNode->{$Subdir} ||= {})
            }

            if ($ChangeType eq 'R' && $SourcePath)
            {
                $ParentNode->{$Name} = {'/' => 1};

                $Self->SetUpstreamAndSynchRev($SourcePath,
                    $BranchDir, $SourceRevision)
            }
            elsif (my $DeletedNode = delete $ParentNode->{$Name})
            {
                my $ObsoletePathTree = ($Self->{ObsoleteBranchPaths} ||= {});

                for my $Subdir (@Subdirs)
                {
                    $ObsoletePathTree = ($ObsoletePathTree->{$Subdir} ||= {})
                }

                MergeDeletedTrees($ObsoletePathTree, $Name, $DeletedNode)
            }
        }
        elsif ($ChangeType eq 'A' && $SourcePath)
        {
            NCBI::SVN::Branching::Util::FindTreeNode($BranchStructure,
                $BranchDir)->{'/'} = 1;

            $Self->SetUpstreamAndSynchRev($SourcePath,
                $BranchDir, $SourceRevision)
        }
    }
}

sub FindPathsInTreeRecursively
{
    my ($Paths, $Path, $Tree) = @_;

    while (my ($Name, $Subtree) = each %$Tree)
    {
        unless ($Name eq '/')
        {
            FindPathsInTreeRecursively($Paths, "$Path/$Name", $Subtree)
        }
        else
        {
            push @$Paths, $Path
        }
    }
}

sub FindPathsInTree
{
    my ($Tree) = @_;

    my @Paths;

    while (my ($Name, $Subtree) = each %$Tree)
    {
        FindPathsInTreeRecursively(\@Paths, $Name, $Subtree)
    }

    return \@Paths
}

sub new
{
    my ($Class, $RootURL, $BranchPath, $MaxBranchRev) = @_;

    $MaxBranchRev ||= 'HEAD';

    my @BranchRevisions;
    my @MergeDownRevisions;

    my $Self = $Class->SUPER::new(
        BranchPath => $BranchPath,
        BranchRevisions => \@BranchRevisions,
        MergeDownRevisions => \@MergeDownRevisions
    );

    print STDERR "Gathering information about $BranchPath...\n";

    my $RevisionLog = $Self->{SVN}->ReadLog('--stop-on-copy',
        "-r$MaxBranchRev\:1", $RootURL, $BranchPath);

    my %BranchStructure;
    my $CommonTarget = "/$BranchPath/";

    for my $Revision (@$RevisionLog)
    {
        push @BranchRevisions, $Revision;

        if ($Revision->{LogMessage} =~ m/^Created branch '(.+?)'.$/o &&
            $1 eq $BranchPath)
        {
            $Self->{BranchCreationRevision} = $Revision;

            $Self->ModelBranchStructure(\%BranchStructure,
                $Revision, $CommonTarget);

            last
        }
    }

    unless ($Self->{UpstreamPath} &&
        ($Self->{BranchSourceRevision} = $Self->{LastSynchRevision}))
    {
        die 'unable to determine branch source'
    }

    for my $Revision (reverse @BranchRevisions)
    {
        my $LogMessage = $Revision->{LogMessage};

        if ($LogMessage =~ m/^Modified branch '(.+?)'.$/o && $1 eq $BranchPath)
        {
            $Self->ModelBranchStructure(\%BranchStructure,
                $Revision, $CommonTarget)
        }
        elsif ($LogMessage =~
            m/^Merged changes up to r(\d+) from '.+' into '(.+)'.$/o &&
                $2 eq $BranchPath)
        {
            unshift @MergeDownRevisions,
                [$Revision, $Self->{LastSynchRevision} = $1]
        }
    }

    my @BranchDirs = sort @{FindPathsInTree(\%BranchStructure)};

    $Self->{BranchDirs} = \@BranchDirs;

    if ($Self->{ObsoleteBranchPaths})
    {
        ClearDeletedTree($Self->{ObsoleteBranchPaths}, \%BranchStructure);

        $Self->{ObsoleteBranchPaths} =
            FindPathsInTree($Self->{ObsoleteBranchPaths})
    }

    $Self->{UpstreamPath} =~ s/^\/?(.+?)\/?$/$1/;

    return $Self
}

package NCBI::SVN::BranchAndUpstreamInfo;

use base qw(NCBI::SVN::BranchInfo);

sub new
{
    my ($Class, $RootURL, $BranchPath, $MaxBranchRev, $MaxUpstreamRev) = @_;

    my $Self = $Class->SUPER::new($RootURL, $BranchPath, $MaxBranchRev);

    $MaxUpstreamRev ||= 'HEAD';

    my $UpstreamPath = $Self->{UpstreamPath};

    print STDERR "Gathering information about $UpstreamPath...\n";

    my $UpstreamRevisions = $Self->{SVN}->ReadLog('--stop-on-copy',
        "-r$MaxUpstreamRev\:$Self->{BranchRevisions}->[-1]->{Number}",
        $RootURL, map {"$UpstreamPath/$_"} @{$Self->{BranchDirs}});

    my @MergeUpRevisions;

    for my $Revision (@$UpstreamRevisions)
    {
        if ($Revision->{LogMessage} =~
            m/^Merged changes up to r(\d+) from '(.+)' into '.+'.$/o &&
                $2 eq $BranchPath)
        {
            push @MergeUpRevisions, [$Revision, $1]
        }
    }

    @$Self{qw(UpstreamRevisions MergeUpRevisions)} =
        ($UpstreamRevisions, \@MergeUpRevisions);

    return $Self
}

1
