#include "grid.h"
#include <Ogre.h>
using namespace Ogre;

OgreGrid::OgreGrid(SceneManager *scene, String name):
    mScene(scene),
    mAttachedNode(0),
    material(name)
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

void OgreGrid::attachToNode(SceneNode *node)
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
    ManualObject* o = mGrid;

    o->setRenderQueueGroup(drawQueue);

    // Offset and cell size
    mOwnNode->setPosition(ofX, ofY, ofZ);
    mOwnNode->setScale(csX, csY, csZ);

    o->clear();
    o->begin("cyan_material", RenderOperation::OT_LINE_LIST);

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

    o->begin("red_material", RenderOperation::OT_LINE_LIST);

    // Draw half the center line with red color
    o->position(0, 0, 0);
    o->position(0, 0, zmin);

    o->end();
}
