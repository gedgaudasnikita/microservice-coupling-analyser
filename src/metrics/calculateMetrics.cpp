#include <vector>
#include <cstring>
#include "calculateMetrics.h"

float calculateCCOS(InputServiceStructureModel serviceStructure) {
  std::vector<float> NCOSvalues;

  for (
    std::vector<Service>::iterator it = serviceStructure.services.begin() ; 
    it != serviceStructure.services.end(); 
    ++it
  ) {
    Service s = (*it);
    float NCOS = 0;

    for (
      std::vector<Service>::iterator it_i = serviceStructure.services.begin() ; 
      it_i != serviceStructure.services.end(); 
      ++it_i
    ) {
      Service s_i = (*it_i);
      for (
        std::vector<ServiceDependency>::iterator it_x = serviceStructure.serviceDependencies.begin() ; 
        it_x != serviceStructure.serviceDependencies.end(); 
        ++it_x
      ) {
        ServiceDependency s_si = (*it_x);
        if (
          s_si.fromServiceName.compare(s.name) == 0 &&
          s_si.toServiceName.compare(s_i.name) == 0
        ) {
          NCOS++;
          break;
        }
      }
    }
    
    NCOSvalues.push_back(NCOS);
  }

  float NCOSsum = 0;
  for (
    std::vector<float>::iterator it = NCOSvalues.begin(); 
    it != NCOSvalues.end(); 
    ++it
  ) {
    NCOSsum += *it;
  }

  return NCOSsum / (NCOSvalues.size() * NCOSvalues.size() - NCOSvalues.size());
}

float calculateMaxNCOS(InputServiceStructureModel serviceStructure) {
  std::vector<float> NCOSvalues;

  for (
    std::vector<Service>::iterator it = serviceStructure.services.begin() ; 
    it != serviceStructure.services.end(); 
    ++it
  ) {
    Service s = (*it);
    float NCOS = 0;

    for (
      std::vector<Service>::iterator it_i = serviceStructure.services.begin() ; 
      it_i != serviceStructure.services.end(); 
      ++it_i
    ) {
      Service s_i = (*it_i);
      for (
        std::vector<ServiceDependency>::iterator it_x = serviceStructure.serviceDependencies.begin() ; 
        it_x != serviceStructure.serviceDependencies.end(); 
        ++it_x
      ) {
        ServiceDependency s_si = (*it_x);
        if (
          s_si.fromServiceName.compare(s.name) == 0 &&
          s_si.toServiceName.compare(s_i.name) == 0
        ) {
          NCOS++;
          break;
        }
      }
    }
    
    NCOSvalues.push_back(NCOS);
  }

  float maxNCOS = 0;
  for (
    std::vector<float>::iterator it = NCOSvalues.begin(); 
    it != NCOSvalues.end(); 
    ++it
  ) {
    maxNCOS = (*it) > maxNCOS ? (*it) : maxNCOS;
  }

  return maxNCOS;
}

float calculateMaxNCIS(InputServiceStructureModel serviceStructure) {
  std::vector<float> NCISvalues;

  for (
    std::vector<Service>::iterator it = serviceStructure.services.begin() ; 
    it != serviceStructure.services.end(); 
    ++it
  ) {
    Service s = (*it);
    float NCIS = 0;

    for (
      std::vector<Service>::iterator it_i = serviceStructure.services.begin() ; 
      it_i != serviceStructure.services.end(); 
      ++it_i
    ) {
      Service s_i = (*it_i);
      for (
        std::vector<ServiceDependency>::iterator it_x = serviceStructure.serviceDependencies.begin() ; 
        it_x != serviceStructure.serviceDependencies.end(); 
        ++it_x
      ) {
        ServiceDependency s_si = (*it_x);
        if (
          s_si.fromServiceName.compare(s_i.name) == 0 &&
          s_si.toServiceName.compare(s.name) == 0
        ) {
          NCIS++;
          break;
        }
      }
    }
    
    NCISvalues.push_back(NCIS);
  }

  float maxNCIS = 0;
  for (
    std::vector<float>::iterator it = NCISvalues.begin(); 
    it != NCISvalues.end(); 
    ++it
  ) {
    maxNCIS = (*it) > maxNCIS ? (*it) : maxNCIS;
  }

  return maxNCIS;
}

MetricResults calculateMetrics(InputServiceStructureModel serviceStructure) {
  MetricResults result;

  result.CCOS = calculateCCOS(serviceStructure);
  result.maxNCIS = calculateMaxNCIS(serviceStructure);
  result.maxNCOS = calculateMaxNCOS(serviceStructure);

  return result;
}
