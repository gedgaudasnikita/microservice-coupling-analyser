#ifndef PARSERS_H
#define PARSERS_H

#include "../dataModels/dataModels.h"

InputServiceStructureModel parseDocker(std::string workingDir);
InputServiceStructureModel parseKubernetes(std::string workingDir);

#endif
