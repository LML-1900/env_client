//
// Created by lml on 23-8-8.
//

#ifndef ENV_CLIENT_ENVMIDWARE_H
#define ENV_CLIENT_ENVMIDWARE_H
#include <iostream>
#include <string>
#include <Poco/Base64Decoder.h>
#include <sstream>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <google/protobuf/repeated_field.h>
#include <grpcpp/channel.h>
#include "env_service.grpc.pb.h"
#include "Dem.h"
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using grpc::Channel;
using environmentdata::EnvironmentData;
using environmentdata::Area;
using environmentdata::GetDataRequest;
using environmentdata::GetDataResponse;
using environmentdata::CraterArea;
using environmentdata::DataType;
using environmentdata::Crater;
using environmentdata::Position;
using environmentdata::StartStopPoints;
using environmentdata::RoutePoints;

class EnvMidWare {
public:
    EnvMidWare(std::shared_ptr<Channel> channel);

    void CallGetDataRPC(const Area &area, int level, int dataType, std::string time = "1645671600");

    bool CallUpdateCraterRPC(const Crater &crater);

    void CallGetRoutePointsRPC(const StartStopPoints &startStopPoints);

    Area makeArea(Store::Position bottomLeft, Store::Position topRight);

    Position makePosition(double longitude, double latitude);

    Crater makeCrater(Store::Position position, double width, double depth);

    StartStopPoints makeNavigatePointsPair(Store::Position startPoint, Store::Position endPoint);
private:
    std::unique_ptr<EnvironmentData::Stub> stub_;
    std::string decodeBase64ToString(const std::string& base64String);

};


#endif //ENV_CLIENT_ENVMIDWARE_H
