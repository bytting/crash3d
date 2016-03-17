#ifndef _materials_h_
#define _materials_h_

#include <string>
#include <map>
#include <Ogre.h>

extern std::map<std::string, Ogre::MaterialPtr> materials;

void CreateMaterials();
void DestroyMaterials();

#endif // _materials_h_