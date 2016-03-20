/*
Copyright (c) 2013 Peter Petrov, Pi-Dev, Bulgaria
Inspired by Jeroen Dierckx's EditorGridSystem, but rewritten from scratch.
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Code in this file is based on Peter's Grid System
http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Peter%27s+Grid+System
Copyright (c) 2016 Dag Robole
*/

#ifndef _grid_h_
#define _grid_h_

#include <Ogre.h>

class OgreGrid
{
private:

    OgreGrid();

public:

    explicit OgreGrid(Ogre::SceneManager* scene, Ogre::String materialName = "Template/Cyan");
    ~OgreGrid();

    Ogre::ManualObject *mGrid;
    Ogre::SceneNode *mOwnNode, *mAttachedNode;
    Ogre::SceneManager *mScene;
    Ogre::String mMaterialName;

    // Cell sizes
    float csX, csY, csZ;

    // Grid's (0,0,0) offset
    float ofX, ofY, ofZ;

    // Grid size
    int xmin, xmax, zmin, zmax;

    // The RenderQueue - RENDER_QUEUE_MAIN/OVERLAY/BACKGROUND
    int drawQueue;

    inline void setCellSize(float size) { csX = csY = csZ = size; }
    inline void setCellSize(float x, float y, float z) { csX = x; csY = y, csZ = z; }
    inline void setOffset(float x, float y, float z) { ofX = x, ofY = y, ofZ = z; }

    void attachToNode(Ogre::SceneNode* node);

    //After setup, call this to regenerate the grid
    void update();

    void hide();
    void show();
};

#endif // _grid_h_
