#include <fstream>
#include <iostream>
#include <cstring>
#include <exception>
#include <experimental/filesystem>
#include "./parsers.h"
#include "../../lib/yaml-cpp/include/yaml-cpp/eventhandler.h"
#include "../../lib/yaml-cpp/include/yaml-cpp/yaml.h"

InputServiceStructureModel parseDocker(std::string workingDir) {
  InputServiceStructureModel inputModel;

  for (auto & file : std::experimental::filesystem::directory_iterator(workingDir)) {
    InputServiceStructureModel localInputModel;
    std::ifstream fin;
    fin.open(file.path());
    
    try {
      YAML::Node config = YAML::Load(fin);
      YAML::Node services = config["services"];
      if (services.IsDefined() && services.IsMap()) {
        for (const auto serviceEntry : services) {
          Service newService;
          newService.name = serviceEntry.first.as<std::string>();
          localInputModel.services.push_back(newService);
         
          YAML::Node links = serviceEntry.second["links"];
          if (links.IsDefined() && links.IsSequence()) {
            for (const auto linkEntry : links) {
              ServiceDependency newServiceDependency;
              newServiceDependency.fromServiceName = serviceEntry.first.as<std::string>();
              newServiceDependency.toServiceName = linkEntry.as<std::string>();
              localInputModel.serviceDependencies.push_back(newServiceDependency);
            }
          }
        }
      }
    } catch(std::exception& e) {}
    
    fin.close();
    inputModel = mergeModels(inputModel, localInputModel);
  }

  return inputModel;
}

InputServiceStructureModel parseKubernetes(std::string  workingDir) {
  InputServiceStructureModel inputModel;

  for (auto & file : std::experimental::filesystem::directory_iterator(workingDir)) {
    InputServiceStructureModel localInputModel;
    std::ifstream fin;
    fin.open(file.path());
    
    try {
      YAML::Node config = YAML::Load(fin);
      YAML::Node kindNode = config["kind"];
      if (kindNode.IsDefined()) {
        std::string kind = kindNode.as<std::string>();

        if (kind.compare("NetworkPolicy") == 0) {
          std::vector<std::string> ingressServices;
          std::vector<std::string> egressServices;

          YAML::Node specNode = config["spec"];
          if (specNode.IsDefined()) {     
            YAML::Node ingressNode = specNode["ingress"];    
            YAML::Node egressNode = specNode["egress"]; 

            if (
              ingressNode.IsDefined() && 
              egressNode.IsDefined()
            ) {
              for (const auto ingressRule : ingressNode) {
                YAML::Node fromNode = ingressRule["from"]; 

                if (fromNode.IsDefined()) {
                  for (const auto fromRule : fromNode) {
                    YAML::Node podSelectorNode = fromRule["podSelector"];

                    if (podSelectorNode.IsDefined()) {
                      YAML::Node matchLabelsNode = podSelectorNode["matchLabels"];

                      if (matchLabelsNode.IsDefined()) {
                        YAML::Node nameNode = matchLabelsNode["name"];
                        
                        if (nameNode.IsDefined()) {
                          ingressServices.push_back(nameNode.as<std::string>());
                        }
                      }
                    }
                  }
                }
              }
              
              for (const auto egressRule : egressNode) {
                YAML::Node toNode = egressRule["to"]; 

                if (toNode.IsDefined()) {
                  for (const auto toRule : toNode) {
                    YAML::Node podSelectorNode = toRule["podSelector"];

                    if (podSelectorNode.IsDefined()) {
                      YAML::Node matchLabelsNode = podSelectorNode["matchLabels"];

                      if (matchLabelsNode.IsDefined()) {
                        YAML::Node nameNode = matchLabelsNode["name"];
                        
                        if (nameNode.IsDefined()) {
                          egressServices.push_back(nameNode.as<std::string>());
                        }
                      }
                    }
                  }
                }
              }
            }      
          }

          for (const auto ingressService: ingressServices) {
            for (const auto egressService: egressServices) {
              ServiceDependency newServiceDependency;
              newServiceDependency.fromServiceName = ingressService;
              newServiceDependency.toServiceName = egressService;
              localInputModel.serviceDependencies.push_back(newServiceDependency);
            }
          }
        } else if (kind.compare("Pod") == 0) {
          YAML::Node metadataNode = config["metadata"];

          if (metadataNode.IsDefined()) {
            YAML::Node nameNode = metadataNode["name"];

            if (nameNode.IsDefined()) {
              Service newService;
              newService.name = nameNode.as<std::string>();
              localInputModel.services.push_back(newService);
            }
          }

        }
      }
    } catch(std::exception& e) {

        std::cout << e.what() << std::endl;
    }
    
    fin.close();
    inputModel = mergeModels(inputModel, localInputModel);
  }

  return inputModel;
}
