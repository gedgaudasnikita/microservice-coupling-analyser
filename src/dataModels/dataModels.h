#ifndef DATAMODELS_H
#define DATAMODELS_H

#include <vector>
#include <string>
#include <iostream>

struct Service {
  std::string name;
};

struct ServiceDependency {
  std::string fromServiceName;
  std::string toServiceName;
};

struct InputServiceStructureModel {
  std::vector<Service> services;
  std::vector<ServiceDependency> serviceDependencies; 
};

struct MetricResults {
  long double maxNCOS;
  long double maxNCIS;
  long double CCOS;
};

std::ostream& operator<<(std::ostream& os, const InputServiceStructureModel& inputModel);

std::ostream& operator<<(std::ostream& os, const MetricResults& inputModel);

bool areServicesEqual(const Service &left, const Service &right);

bool areServiceDependenciesEqual(const ServiceDependency &left, const ServiceDependency &right);

InputServiceStructureModel mergeModels(InputServiceStructureModel left, InputServiceStructureModel right);

#endif