#include "materials.h"
using namespace Ogre;

void CreateMaterials()
{
	MaterialPtr mat = MaterialManager::getSingleton().create("red_material", "General");
    mat->setReceiveShadows(false);
    mat->getTechnique(0)->setLightingEnabled(true);
    mat->getTechnique(0)->getPass(0)->setDiffuse(1, 0, 0, 0);
    mat->getTechnique(0)->getPass(0)->setAmbient(1, 0, 0);
    mat->getTechnique(0)->getPass(0)->setSelfIllumination(1, 0, 0);	

	mat = MaterialManager::getSingleton().create("green_material", "General");
    mat->setReceiveShadows(false);
    mat->getTechnique(0)->setLightingEnabled(true);
    mat->getTechnique(0)->getPass(0)->setDiffuse(0, 1, 0, 0);
    mat->getTechnique(0)->getPass(0)->setAmbient(0, 1, 0);
    mat->getTechnique(0)->getPass(0)->setSelfIllumination(0, 1, 0);	

	mat = MaterialManager::getSingleton().create("cyan_material", "General");
    mat->setReceiveShadows(false);
    mat->getTechnique(0)->setLightingEnabled(true);
    mat->getTechnique(0)->getPass(0)->setDiffuse(0, 1, 1, 0);
    mat->getTechnique(0)->getPass(0)->setAmbient(0, 1, 1);
    mat->getTechnique(0)->getPass(0)->setSelfIllumination(0, 1, 1);
}
