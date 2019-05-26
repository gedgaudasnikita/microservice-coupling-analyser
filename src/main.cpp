#include <iostream> 
#include <cstring>
#include "./dataModels/dataModels.h"
#include "metrics/calculateMetrics.h"
#include "parsers/parsers.h"

#define DOCKER_ORCHESTRATION_TYPE "docker"
#define KUBERNETES_ORCHESTRATION_TYPE "kubernetes"

int main(int argc, char* argv[]) {
  int ret = 0;

  std::string orchestrationType = "";
  std::string workingDir = "./";

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      if (strcmp(argv[i], "-h") == 0) {
        std::cout << "Supported parameters: " << std::endl;
        std::cout << '\t' << "--type=" << DOCKER_ORCHESTRATION_TYPE << "|" << KUBERNETES_ORCHESTRATION_TYPE << '\t' << "select the relevant orchestration type" << std::endl;
        std::cout << '\t' << "[--working-dir=path]"  << '\t' << "select the working directory to analyze, defaults to current working directory" << std::endl;
      } else if (strstr(argv[i], "--type")) {
        strtok(argv[i], "=");
        char* value = strtok(NULL, "=");
        orchestrationType = value;
      } else if (strstr(argv[i], "--working-dir")) {
        strtok(argv[i], "=");
        char* value = strtok(NULL, "=");
        workingDir = value;
      }
    }
  }

  InputServiceStructureModel inputModel;

  if (orchestrationType.compare(DOCKER_ORCHESTRATION_TYPE) == 0) {
    inputModel = parseDocker(workingDir);
  } else if (orchestrationType.compare(KUBERNETES_ORCHESTRATION_TYPE) == 0) {
    inputModel = parseKubernetes(workingDir);
  } else {
    std::cerr << "Unsupported orchestration type: '" << orchestrationType << "' . Start the program with a '-h' flag for more info." << std::endl;
    return -1;
  }

  std::cout << inputModel;

  MetricResults result = calculateMetrics(inputModel);

  std::cout << result;

  return ret;
} 