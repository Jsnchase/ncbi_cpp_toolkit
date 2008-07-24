#ifndef CORELIB___TEST_BOOST__HPP
#define CORELIB___TEST_BOOST__HPP

/*  $Id$
 * ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government have not placed any restriction on its use or reproduction.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the NLM and the U.S.
 *  Government do not and cannot warrant the performance or results that
 *  may be obtained by using this software or data. The NLM and the U.S.
 *  Government disclaim all warranties, express or implied, including
 *  warranties of performance, merchantability or fitness for any particular
 *  purpose.
 *
 *  Please cite the author in any work or product based on this material.
 *
 * ===========================================================================
 *
 * Author:  Pavel Ivanov
 *
 */

/// @file test_boost.hpp
///   Utility stuff for more convenient using of Boost.Test library.
///
/// This header must be included before any Boost.Test header
/// (if you have any).


#include <corelib/ncbistd.hpp>

/// Redefinition of the name of global function used in Boost.Test for
/// initialization. It is called now from NCBI wrapper function called before
/// user-defined.
#define init_unit_test_suite  NcbiInitUnitTestSuite


// Keep Boost's inclusion of <limits> from breaking under old WorkShop versions.
#if defined(numeric_limits)  &&  defined(NCBI_NUMERIC_LIMITS)
#  undef numeric_limits
#endif

// By default BOOST_AUTO_TEST_MAIN will be defined
#ifndef NCBI_BOOST_NO_AUTO_TEST_MAIN
#  ifndef BOOST_AUTO_TEST_MAIN
#    define BOOST_AUTO_TEST_MAIN
#  endif
#endif

#include <boost/test/auto_unit_test.hpp>


// Redefine some Boost macros to make them more comfortable
#undef BOOST_CHECK_THROW_IMPL
#undef BOOST_CHECK_NO_THROW_IMPL

#define BOOST_CHECK_THROW_IMPL( S, E, P, prefix, TL )                    \
try {                                                                    \
    BOOST_TEST_PASSPOINT();                                              \
    S;                                                                   \
    BOOST_CHECK_IMPL( false, "exception " BOOST_STRINGIZE( E )           \
                             " is expected", TL, CHECK_MSG ); }          \
catch( E const& ex ) {                                                   \
    boost::unit_test::ut_detail::ignore_unused_variable_warning( ex );   \
    BOOST_CHECK_IMPL( P, prefix BOOST_STRINGIZE( E ) " is caught",       \
                      TL, CHECK_MSG );                                   \
}                                                                        \
catch (...) {                                                            \
    BOOST_CHECK_IMPL(false, "an unexpected exception was thrown by "     \
                            BOOST_STRINGIZE( S ),                        \
                     TL, CHECK_MSG);                                     \
}                                                                        \
/**/

#define BOOST_CHECK_NO_THROW_IMPL( S, TL )                                   \
try {                                                                        \
    S;                                                                       \
    BOOST_CHECK_IMPL( true, "no exceptions thrown by " BOOST_STRINGIZE( S ), \
                      TL, CHECK_MSG );                                       \
}                                                                            \
catch (std::exception& ex) {                                                 \
    BOOST_CHECK_IMPL( false, "an std::exception was thrown by "              \
                             BOOST_STRINGIZE( S ) " : " << ex.what(),        \
                      TL, CHECK_MSG);                                        \
}                                                                            \
catch( ... ) {                                                               \
    BOOST_CHECK_IMPL( false, "a nonstandard exception thrown by "            \
                             BOOST_STRINGIZE( S ),                           \
                      TL, CHECK_MSG );                                       \
}                                                                            \
/**/


/** @addtogroup Tests
 *
 * @{
 */


BEGIN_NCBI_SCOPE


/// Type of function with unit tests initialization code
typedef void (*TNcbiBoostInitFunc)(void);

/// Macro defining initialization function which will be called before
/// tests execution. The usage of this macro is similar to
/// BOOST_AUTO_TEST_CASE, i.e.:
/// BOOST_AUTO_INITIALIZATION(FunctionName)
/// {
///     // initialization function body
/// }
#define BOOST_AUTO_INITIALIZATION(name)                                     \
static void name(void);                                                     \
static ::NCBI_NS_NCBI::SNcbiBoostIniter BOOST_JOIN(name, _initer) (&name);  \
static void name(void)

/// Registrar of all initialization functions
void RegisterNcbiBoostInit(TNcbiBoostInitFunc func);

/// Class for implementing automatic registration of init-functions
struct SNcbiBoostIniter
{
    SNcbiBoostIniter(TNcbiBoostInitFunc func)
    {
        RegisterNcbiBoostInit(func);
    }
};

/// Mark test case/suite as dependent on another test case/suite.
/// If dependency test case didn't executed successfully for any reason then
/// dependent test will not be executed. This rule has one exception: if test
/// is requested to execute in command line via parameter "--run_test" and
/// dependency was not requested to execute, requested test will be executed
/// anyways.
///
/// @param tu
///   Test case/suite that should be marked as dependent
/// @param dep_tu
///   Test case/suite that will be "parent" for tu
void NcbiTestDependsOn(boost::unit_test::test_unit* tu,
                       boost::unit_test::test_unit* dep_tu);

/// Disable test unit.
/// Disabled test unit will not be executed (as if p_enabled is set to false)
/// but it will be reported in final Boost.Test report as disabled (as opposed
/// to setting p_enabled to false when test does not appear in final
/// Boost.Test report).
void NcbiTestDisable(boost::unit_test::test_unit* tu);


END_NCBI_SCOPE


/* @} */

#endif  /* CORELIB___TEST_BOOST__HPP */
