//
// Created by lml on 23-8-8.
//

#include "EnvMidWare.h"

EnvMidWare::EnvMidWare(std::shared_ptr<Channel> channel): stub_(EnvironmentData::NewStub(channel)) {}

void EnvMidWare::CallGetDataRPC(const environmentdata::Area &area, int level, int dataType, std::string time) {
    GetDataRequest request;
    request.set_datatype(DataType(dataType));
    request.set_level(level);
    request.set_time(time);
    request.mutable_area()->CopyFrom(area);
    GetDataResponse dataResponse;
    ClientContext context;
    std::unique_ptr<ClientReader<GetDataResponse> > reader(stub_->GetData(&context, request));
    int count = 0;
    while (reader->Read(&dataResponse)) {
        count++;
//        std::cout << "返回的瓦片ID：" << dataResponse.tileid() << std::endl;
        std::string decodeContent = decodeBase64ToString(dataResponse.content());
        //std::cout << "content: " << decodeContent << std::endl;
//        int craterSize = dataResponse.craters_size();
//        std::vector<Store::Crater> craters(craterSize);
//        for (int i = 0; i < craterSize; i++) {
//            Store::Crater crater;
//            Crater craterFromServer = dataResponse.craters(i);
//            crater.position.longitude = craterFromServer.pos().longitude();
//            crater.position.latitude = craterFromServer.pos().latitude();
//            crater.width = craterFromServer.width();
//            crater.depth = craterFromServer.depth();
//            printf("crater %d: pos: %lf-%lf\twidth:%lf\tdepth:%lf\n", i,
//                   crater.position.longitude, crater.position.latitude, crater.width, crater.depth);
//        }
//        std::cout << std::endl;
    }
    Status status = reader->Finish();
    if (status.ok()) {
        std::cout << "数据分发完成,count: " << count << std::endl;
    }
}

std::string EnvMidWare::decodeBase64ToString(const std::string& base64String) {
    std::istringstream istr(base64String);
    Poco::Base64Decoder decoder(istr);

    std::ostringstream ostr;
    std::copy(std::istreambuf_iterator<char>(decoder), {}, std::ostreambuf_iterator<char>(ostr));

    return ostr.str();
}

Position EnvMidWare::makePosition(double longitude, double latitude) {
    Position pos;
    pos.set_longitude(longitude);
    pos.set_latitude(latitude);
    return pos;
}

Area EnvMidWare::makeArea(Store::Position bottomLeft, Store::Position topRight) {
    Area area;
    area.mutable_bottomleft()->CopyFrom(makePosition(bottomLeft.longitude, bottomLeft.latitude));
    area.mutable_topright()->CopyFrom(makePosition(topRight.longitude, topRight.latitude));
    return area;
}

Crater EnvMidWare::makeCrater(Store::Position position, double width, double depth) {
    Crater crater;
    crater.mutable_pos()->CopyFrom(makePosition(position.longitude, position.latitude));
    crater.set_width(width);
    crater.set_depth(depth);
    return crater;
}

StartStopPoints EnvMidWare::makeNavigatePointsPair(Store::Position startPoint, Store::Position endPoint) {
    StartStopPoints startStopPoints;
    startStopPoints.mutable_start()->CopyFrom(makePosition(startPoint.longitude, startPoint.latitude));
    startStopPoints.mutable_end()->CopyFrom(makePosition(endPoint.longitude, endPoint.latitude));
    return startStopPoints;
}

bool EnvMidWare::CallUpdateCraterRPC(const environmentdata::Crater &crater) {
    ClientContext context;
    CraterArea craterArea;
    Status status = stub_->UpdateCrater(&context, crater, &craterArea);
    if (!status.ok()) {
        std::cout << "fail to update crater, err:" << status.error_message() << std::endl;
        return false;
    }
    printf("send a crater to server: lon:%lf\tlat:%lf\twidth:%lf\tdepth:%lf\n", crater.pos().longitude(), crater.pos().latitude(), crater.width(), crater.depth());
    return true;
}

void EnvMidWare::CallGetRoutePointsRPC(const environmentdata::StartStopPoints &startStopPoints) {
    ClientContext context;
    RoutePoints points;
    Status status = stub_->GetRoutePoints(&context, startStopPoints, &points);
    if (status.ok()) {
        int pointsSize = points.pos_size();
        std::cout << pointsSize << std::endl;
        for (int i = 0; i < pointsSize; i++) {
            std::cout << points.pos(i).longitude() << " " << points.pos(i).latitude() << std::endl;
        }
    }
}