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
* Authors:  Paul Thiessen
*
* File Description:
*      Classes to hold the OpenGL rendering engine
*
* ---------------------------------------------------------------------------
* $Log$
* Revision 1.2  2000/07/12 23:28:27  thiessen
* now draws basic CPK model
*
* Revision 1.1  2000/07/12 14:10:45  thiessen
* added initial OpenGL rendering engine
*
* ===========================================================================
*/

#ifndef CN3D_OPENGL_RENDERER__HPP
#define CN3D_OPENGL_RENDERER__HPP

// do not include GL headers here, so that other modules can more easily
// access this without potential name conflicts

#include "cn3d/vector_math.hpp"

BEGIN_SCOPE(Cn3D)

// non-object functions to draw primitives in OpenGL
void DrawSphere(const Vector& site, double radius, const Vector& color);

// the OpenGLRenderer class

class StructureSet;

class OpenGLRenderer
{
public:
    OpenGLRenderer(void);

    // public data

    // public methods

    // calls once-only OpenGL initialization stuff (should be called after
    // the rendering context is established and the renderer made current)
    void Init(void) const;

    // sets the size of the viewport
    void SetSize(int width, int height) const;

    // tells the renderer that new camera settings need to be applied
    void NewView(void) const;

    // reset camera to original state
    void ResetCamera(void);

    // called to change view (according to mouse movements)
    enum eViewAdjust {
        eXYRotateHV,        // rotate about X,Y axes according to horiz. & vert. movement
        eZRotateH,          // rotate in plane (about Z) according to horiz. movement
        eScaleH,            // scale according to horiz. movement
        eXYTranslateHV,     // translate in X,Y according to horiz. & vert. movement
        eZoomHHVV,          // zoom according to (H1,V1),(H2,V2) box
        eZoomIn,            // zoom in
        eZoomOut            // zoom out
    };
    void ChangeView(eViewAdjust control, int dX = 0, int dY = 0, int X2 = 0, int Y2 = 0);

    // draws the display lists
    void Display(void) const;

    // tells the renderer what structure(s) it's to draw
    void AttachStructureSet(StructureSet *targetStructureSet);

    // constructs the structure display lists (but doesn't draw them)
    void Construct(void);

private:
    StructureSet *structureSet;

    // camera data
    double cameraDistance, cameraAngleRad, 
        cameraLookAtX, cameraLookAtY,
        cameraClipNear, cameraClipFar,
        viewMatrix[16];

    // controls for view changes
    double rotateSpeed;

    // background color
    float background[3];
};

END_SCOPE(Cn3D)

#endif // CN3D_OPENGL_RENDERER__HPP
