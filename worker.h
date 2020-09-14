/*
   Copyright 2013 Fabien Pierre-Nicolas.
      - Primarily authored by Fabien Pierre-Nicolas

   This file is part of multiple-methods-qt-thread-example, a simple example to demonstrate how to use a thread running multiple methods.
   This example is explained on http://fabienpn.wordpress.com/qt-thread-multiple-methods-with-sources/

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This progra is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>


#include "Poco/Net/Net.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Exception.h"
#include "Poco/Timer.h"
#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
#include "Poco/NumericString.h"
#include <iostream>
#include <time.h>

using namespace Poco;
using namespace Poco::Dynamic;
using Poco::Net::SocketAddress;
using Poco::Net::StreamSocket;
using Poco::Net::Socket;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::Thread;
using Poco::Stopwatch;
using namespace std;

const std::string hostname = "192.168.0.117"; //STEP2 IP Address Monkey
const Poco::UInt16 PORT = 32452;

enum {
    SIZE_HEADER = 52,
    SIZE_COMMAND = 4,
    SIZE_HEADER_COMMAND = 56,
    SIZE_DATA_MAX = 200,
    SIZE_DATA_ASCII_MAX = 32
};

union Data
{
    unsigned char byte[SIZE_DATA_MAX];
    float float6dArr[9];
};

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    /**
     * @brief This enum describes the various available methods
     */
    enum Method {
        Method1 = 1,
        Method2 = 2,
        Method3 = 3
    };
    /**
     * @brief Requests for the method @em method to be executed
     *
     * This method will defines #_method and set #_abort to interrupt current method.
     * It is thread safe as it uses #mutex to protect access to #_method and #_abort variable.
     */
    void requestMethod(Method method);
    /**
     * @brief Requests the process to abort
     *
     * It is thread safe as it uses #mutex to protect access to #_abort variable.
     */
    void abort();

    StreamSocket ss;
private:
    /**
     * @brief Currently requested method
     */
    Method _method;
    /**
     * @brief Process is aborted when @em true
     */
    bool _abort;
    /**
     * @brief Current method is interrupted when @em true
     */
    bool _interrupt;
    /**
     * @brief Protects access to #_abort
     */
    QMutex mutex;
    /**
     * @brief Condition used to wait for a new request to be called in the #mainLoop()
     */
    QWaitCondition condition;
    /**
     * @brief 1st method which could be called
     *
     * Counts 10 sec in this example.
     * Counting is interrupted if #_abort or #_interrupt is set to true.
     */
    void doMethod1();
    /**
     * @brief 2nd method which could be called
     *
     * Counts 20 sec in this example.
     * Counting is interrupted if #_abort or #_interrupt is set to true.
     */
    void doMethod2();
    /**
     * @brief 3rd method which could be called
     *
     * Counts 30 sec in this example.
     * Counting is interrupted if #_abort or #_interrupt is set to true.
     */
    void doMethod3();


    int cnt = 0;
    int flag_return = 0;
    int rpt_cnt = 1;
    clock_t start, check, end, init;
    int motion_buf = 0;
    double glob_time, loop_time;
    Data data_rev, data;
    unsigned char readBuff[1024];
    unsigned char writeBuff[1024];

signals:
    /**
     * @brief This signal is emitted when counted value is changed (every sec)
     */
    void valueChanged(const QString &value);
    /**
     * @brief This signal is emitted when process is finished (either by counting 60 sec or being aborted)
     */
    void finished();

public slots:
    /**
     * @brief Waits for a method to be requested then executes it, then waits again
     */
    void mainLoop();
};

#endif // WORKER_H
