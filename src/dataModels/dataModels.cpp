#include <algorithm>
#include <cstring>
#include <iostream>
#include "./dataModels.h"

bool areServicesEqual(const Service &left, const Service &right) {
  return left.name.compare(right.name) == 0;
}

bool areServiceDependenciesEqual(const ServiceDependency &left, const ServiceDependency &right) {
  return left.fromServiceName.compare(right.fromServiceName) == 0 &&
          left.toServiceName.compare(right.toServiceName) == 0;
}

InputServiceStructureModel mergeModels(InputServiceStructureModel left, InputServiceStructureModel right) {
  InputServiceStructureModel result;

  result.services.insert(
    result.services.end(),
    left.services.begin(),
    left.services.end()
  );

  result.services.insert(
    result.services.end(),
    right.services.begin(),
    right.services.end()
  );

  result.serviceDependencies.insert(
    result.serviceDependencies.end(),
    left.serviceDependencies.begin(),
    left.serviceDependencies.end()
  );

  result.serviceDependencies.insert(
    result.serviceDependencies.end(),
    right.serviceDependencies.begin(),
    right.serviceDependencies.end()
  );

  result.services.erase(
    std::unique(
      result.services.begin(),
      result.services.end(),
      areServicesEqual
    ), 
    result.services.end()
  );
  result.serviceDependencies.erase(
    std::unique(
      result.serviceDependencies.begin(),
      result.serviceDependencies.end(),
      areServiceDependenciesEqual
    ), 
    result.serviceDependencies.end()
  );
  
  return result;
}

std::ostream& operator<<(std::ostream& os, const InputServiceStructureModel& inputModel) {
    os << "Parsed input: " << std::endl;
    
    os << '\t' << "Services: ";
    for (const auto service : inputModel.services) {
      os << service.name << "; ";
    }

    os << std::endl << '\t' << "Service Dependencies:" <<std::endl;

    for (const auto serviceDependency : inputModel.serviceDependencies) {
      os << '\t'<< '\t'<< serviceDependency.fromServiceName << "->" << serviceDependency.toServiceName << std::endl;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const MetricResults& metricResults) {
    os << "Metric values: " << std::endl;
    
    os << '\t' << "CCOS: " << metricResults.CCOS << std::endl;
    os << '\t' << "maxNCIS: " << metricResults.maxNCIS << std::endl;
    os << '\t' << "maxNCOS: " << metricResults.maxNCOS << std::endl;

    return os;
}
