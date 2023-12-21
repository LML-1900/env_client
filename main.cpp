#include <iostream>
#include <chrono>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "env_service.grpc.pb.h"
#include "env_service.pb.h"
#include "EnvMidWare.h"
#include "Dem.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using environmentdata::EnvironmentData;
using environmentdata::Area;
using environmentdata::DataType;
using std::chrono::high_resolution_clock;

int main() {
    EnvMidWare client(
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

//    client.CallUpdateCraterRPC(client.makeCrater(Store::Position(78.42357, 34.45962), 6, 8));
//
    environmentdata::Area area = client.makeArea(Store::Position(78.41, 34.46), Store::Position(78.74, 34.79));
    double sum = 0;
    int n = 100;
    for (int i = 0; i < n; i++) {
        printf("test %d\n", i + 1);
        high_resolution_clock::time_point beginTime = high_resolution_clock::now();
        client.CallGetDataRPC(area, 14, DataType::DEM);
        high_resolution_clock::time_point endTime = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> interval_ms = endTime - beginTime;
        std::cout << "runtime: " << interval_ms.count() << "ms" << std:: endl << std::endl;
        sum += interval_ms.count();
    }
    printf("average runtime is %lf\n", sum / n);


//    environmentdata::StartStopPoints startStopPoints = client.makeNavigatePointsPair(Store::Position(113.5439372, 22.2180642), Store::Position(113.5425177, 22.2252363));
//    client.CallGetRoutePointsRPC(startStopPoints);
    return 0;
}
