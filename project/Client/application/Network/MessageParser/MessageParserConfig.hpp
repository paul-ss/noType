#pragma once

namespace Network {

namespace StringMessageType {

static const std::string InitRequest                  ("InitRequest");
static const std::string InitResponse                 ("InitResponse");
static const std::string ConnectRequest               ("ConnectRequest");
static const std::string ConnectResponse              ("ConnectResponse");
static const std::string StartGameSessionRequest      ("StartGameSessionRequest");
static const std::string StartGameSessionResponse     ("StartGameSessionResponse");
static const std::string GetTextRequest               ("GetTextRequest");
static const std::string GetTextResponse              ("GetTextResponse");
static const std::string RoomStatusRequest            ("RoomStatusRequest");
static const std::string RoomStatusResponse           ("RoomStatusResponse");
static const std::string ValidateWrittenTextRequest   ("ValidateWrittenTextRequest");
static const std::string ValidateWrittenTextResponse  ("ValidateWrittenTextResponse");
static const std::string Error                        ("Error");

}  // namespace StringMessageType

namespace JsonFields {
// NOTE(vendroid): Общие поля
static const std::string CommandType       ("commandType");
static const std::string Status            ("status");
static const std::string ErrorMessage      ("errorMsg");
static const std::string Success           ("Success");
static const std::string Fail              ("Fail");

// NOTE(vendroid): Основные поля
static const std::string ClientUuid        ("clientUUID");
static const std::string Name              ("name");
static const std::string WaitTime          ("waitTime");
static const std::string Text              ("text");
static const std::string TimeFromStart     ("timeFromStart");
static const std::string WrittenText       ("writtenText");
static const std::string RightCount        ("rightCount");
static const std::string PlayersInfo       ("playersInfo");
static const std::string Players           ("players");

  // NOTE(vendroid): Поля сущности игрока
namespace PlayerInfoFields {

static const std::string Name            ("name");
static const std::string Position        ("position");
static const std::string Speed           ("speed");
static const std::string Status          ("status");
static const std::string Play            ("play");
static const std::string Win             ("Win");
static const std::string Wait            ("Wait");

}  // namespace PlayerInfoFields

}  // namespace JsonFields

}  // namespace Network