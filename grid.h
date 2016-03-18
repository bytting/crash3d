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
