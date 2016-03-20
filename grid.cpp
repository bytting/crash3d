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

#include "grid.h"
#include <Ogre.h>

OgreGrid::OgreGrid(Ogre::SceneManager *scene, Ogre::String materialName):
    mScene(scene),
    mAttachedNode(0),
    mMaterialName(materialName)
{
    assert(scene);
    mOwnNode = scene->createSceneNode();
    mGrid = scene->createManualObject();
    mGrid->setDynamic(true);
    mOwnNode->attachObject(mGrid);

    drawQueue = 1;
    // Cell size
    csX = csY = csZ = 1;
    // Offsets
    ofX = ofY = ofZ = 0;
    // Grid size
    xmin = zmin = -10;
    xmax = zmax = 10;    
}

OgreGrid::~OgreGrid()
{
    if(mAttachedNode)
        mAttachedNode->removeChild(mOwnNode);
    mScene->destroyManualObject(mGrid);
    mScene->destroySceneNode(mOwnNode);
}

void OgreGrid::attachToNode(Ogre::SceneNode *node)
{
    // If already attached, deattach it first
    if(mAttachedNode)
        mAttachedNode->removeChild(mOwnNode);
    // Attach to the new node
    mAttachedNode = node;
    node->addChild(mOwnNode);
}

void OgreGrid::hide()
{
    mOwnNode->setVisible(false);
}
void OgreGrid::show()
{
    mOwnNode->setVisible(true);
}

void OgreGrid::update()
{
    Ogre::ManualObject *o = mGrid;

    o->setRenderQueueGroup(drawQueue);

    // Offset and cell size
    mOwnNode->setPosition(ofX, ofY, ofZ);
    mOwnNode->setScale(csX, csY, csZ);

    o->clear();
    o->begin(mMaterialName, Ogre::RenderOperation::OT_LINE_LIST);

    // draw X lines
    for(int x = xmin; x <= xmax; ++x)
    {
        if(x == 0)
            continue;
        o->position(x, 0, zmin);
        o->position(x, 0, zmax);
    }

    // draw Z lines
    for(int z = zmin; z <= zmax; ++z)
    {
        o->position(xmin, 0, z);
        o->position(xmax, 0, z);
    }

    // Draw half the center line with default color
    o->position(0, 0, zmax);
    o->position(0, 0, 0);

    o->end();

    o->begin("Template/Red", Ogre::RenderOperation::OT_LINE_LIST);

    // Draw half the center line with red color
    o->position(0, 0, 0);
    o->position(0, 0, zmin);

    o->end();
}
