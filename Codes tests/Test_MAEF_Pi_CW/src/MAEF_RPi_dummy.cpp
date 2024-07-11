#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include "serialprotocol.h"

enum State {Ready, Pick, Approach, Oscillate, Obstacle, Stabilise, Drop, Return};

// Function to create JSON command
QString createJsonCommand(const QString &action) {
    QJsonObject jsonObject;
    jsonObject["action"] = action;
    QJsonDocument jsonDoc(jsonObject);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

// Function to parse JSON response
QJsonObject parseResponse(const QByteArray &response) {
    QJsonDocument doc = QJsonDocument::fromJson(response);
    return doc.object();
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    SerialProtocol serialProtocol("/dev/ttyUSB0", 9600);

    State currentState = Ready;
    bool responseReceived = false;
    QByteArray response;

    // Connect the signal for new messages
    QObject::connect(&serialProtocol, &SerialProtocol::newMessage, [&](const QByteArray &message) {
        response = message;
        responseReceived = true;
    });

    while (true) {
        switch (currentState) {
            case Ready:
                {
                    QString jsonString = createJsonCommand("init");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["readyOk"].toBool()) {
                        currentState = Pick;
                    }
                }
                break;

            case Pick:
                {
                    QString jsonString = createJsonCommand("pick");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["pickOk"].toBool()) {
                        currentState = Approach;
                    }
                }
                break;

            case Approach:
                {
                    QString jsonString = createJsonCommand("move");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["approachOk"].toBool()) {
                        currentState = Oscillate;
                    }
                }
                break;

            case Oscillate:
                {
                    QString jsonString = createJsonCommand("oscillate");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["oscillateOk"].toBool()) {
                        currentState = Obstacle;
                    }
                }
                break;

            case Obstacle:
                {
                    QString jsonString = createJsonCommand("passObstacle");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["obstaclePassed"].toBool()) {
                        currentState = Stabilise;
                    }
                }
                break;

            case Stabilise:
                {
                    QString jsonString = createJsonCommand("stabilise");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["stabilised"].toBool()) {
                        currentState = Drop;
                    }
                }
                break;

            case Drop:
                {
                    QString jsonString = createJsonCommand("dropPayload");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["payloadDropped"].toBool()) {
                        currentState = Return;
                    }
                }
                break;

            case Return:
                {
                    QString jsonString = createJsonCommand("goBack");
                    serialProtocol.sendMessage(jsonString);

                    // Wait for response
                    while (!responseReceived);
                    responseReceived = false;

                    QJsonObject parsedResponse = parseResponse(response);
                    if (parsedResponse["startPosition"].toBool()) {
                        currentState = Ready;
                    }
                }
                break;

            default:
                qCritical() << "Unknown state!";
                return 1;
        }
    }

    return a.exec();
}