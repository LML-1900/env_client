#include <iostream>
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

int main() {
    EnvMidWare client(
            grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

//    client.CallUpdateCraterRPC(client.makeCrater(Store::Position(78.42357, 34.45962), 6, 8));
//
//    environmentdata::Area area = client.makeArea(Store::Position(78.41, 34.46), Store::Position(78.51, 34.56));
//    client.CallGetDataRPC(area, 14, DataType::DEM);

    environmentdata::StartStopPoints startStopPoints = client.makeNavigatePointsPair(Store::Position(113.5439372, 22.2180642), Store::Position(113.5425177, 22.2252363));
    client.CallGetRoutePointsRPC(startStopPoints);
    return 0;
}
