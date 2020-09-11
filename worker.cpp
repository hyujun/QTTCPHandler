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

#include "worker.h"
#include <QTimer>
#include <QEventLoop>

#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
    _abort = false;
    _interrupt = false;

    try {
        qDebug() << "Trying to connect server...";
        ss.connect(SocketAddress(hostname, PORT));
        Timespan timeout(1, 0);
        while (ss.poll(timeout, Poco::Net::Socket::SELECT_WRITE) == false)
        {
            qDebug() << "Connecting to server...";
        }
    } catch (Poco::Exception& exc)
    {
        cout << "Fail to connect server..." << exc.displayText() << endl;
        //
    }

}

void Worker::requestMethod(Worker::Method method)
{
    qDebug()<<"Request worker Method"<<method<<"in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _interrupt = true;
    _method = method;
    condition.wakeOne();
}

void Worker::abort()
{
    ss.close();
    qDebug()<<"Request worker aborting in Thread "<<thread()->currentThreadId();
    QMutexLocker locker(&mutex);
    _abort = true;
    condition.wakeOne();
}

void Worker::doMethod1()
{
    qDebug()<<"Starting Method1 in Thread "<< thread()->currentThreadId();
    flag_return=0;
    rpt_cnt=0;
    int i=1;
    while(1)
    {
        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug() << "Interrupting Method1 in Thread "<< thread()->currentThreadId();
            break;
        }
        else
        {
            qDebug() << "Thread Counter : " << rpt_cnt << "sec";

            if(flag_return == 0)
            {
                qDebug() << "=========== Please enter the packet ============";
                qDebug() << "Packet scheme\n| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |";
                qDebug() << "0 : Motion number, 1~6 : Desired data, 7 : Null";
                qDebug() << "================================================";

                try {

                    data.float6dArr[0] = 101.0;
                    data.float6dArr[1] = 0.0;
                    data.float6dArr[2] = 0.0;
                    data.float6dArr[3] = 0.0;
                    data.float6dArr[4] = 0.0;
                    data.float6dArr[5] = 0.0;
                    data.float6dArr[6] = 0.0;
                    data.float6dArr[7] = 0.0;

                    memcpy(writeBuff, data.byte, SIZE_DATA_MAX);
                    if ((int)data.float6dArr[0] < 100)
                    {
                        throw data.float6dArr;
                    }
                    ss.sendBytes(writeBuff, 1024);
                }
                catch (int expn) {
                    qDebug() << "[ERROR] : Please check the Motion";
                }

                if (data.float6dArr[6] < 1.5 && (int)data.float6dArr[0] == 106)
                    qDebug() << "[WARNING] : Please enter a trajectory time longer than 1.5s";

                if ((int)data.float6dArr[0] == 101 && motion_buf == 101)
                {
                    qDebug() << "[WARNING] : Homming has already done";
                }
                motion_buf = (int)data.float6dArr[0];

                flag_return = 1;

            }
            else if(flag_return == 1)
            {
                Timespan timeout(0, 100);
                cnt=0;
                while (ss.poll(timeout, Poco::Net::Socket::SELECT_READ) == false)
                {
                    if (cnt >= 5) {
                        flag_return = 1;
                        qDebug() << "No response from server...";
                        break;
                    }
                    qDebug() << "Waiting to receive data...";
                    cnt++;
                }
            }

            rpt_cnt += 1;

            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();

            emit valueChanged(QString::number(i));
        }
    }
}

void Worker::doMethod2()
{
    qDebug()<<"Starting Method2 in Thread "<< thread()->currentThreadId();
    flag_return=0;
    rpt_cnt = 0;
    int i=2;
    while(1)
    {
        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug() << "Interrupting Method2 in Thread "<< thread()->currentThreadId();
            break;
        }
        else
        {
            qDebug() << "Thread Counter : " << rpt_cnt << "sec";

            if(flag_return == 0)
            {
                qDebug() << "=========== Please enter the packet ============";
                qDebug() << "Packet scheme\n| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |";
                qDebug() << "0 : Motion number, 1~6 : Desired data, 7 : Null";
                qDebug() << "================================================";

                try {

                    data.float6dArr[0] = 105.0;
                    data.float6dArr[1] = 0.0;
                    data.float6dArr[2] = 0.0;
                    data.float6dArr[3] = 0.0;
                    data.float6dArr[4] = 0.0;
                    data.float6dArr[5] = 0.0;
                    data.float6dArr[6] = 0.0;
                    data.float6dArr[7] = 0.0;

                    memcpy(writeBuff, data.byte, SIZE_DATA_MAX);
                    if ((int)data.float6dArr[0] < 100)
                    {
                        throw data.float6dArr;
                    }
                    ss.sendBytes(writeBuff, 1024);
                }
                catch (int expn) {
                    qDebug() << "[ERROR] : Please check the Motion";
                }

                if (data.float6dArr[6] < 1.5 && (int)data.float6dArr[0] == 106)
                    qDebug() << "[WARNING] : Please enter a trajectory time longer than 1.5s";

                if ((int)data.float6dArr[0] == 101 && motion_buf == 101)
                {
                    qDebug() << "[WARNING] : Homming has already done";
                }
                motion_buf = (int)data.float6dArr[0];

                flag_return = 2;

            }
            else if(flag_return == 1)
            {
                Timespan timeout(0, 100);
                cnt=0;
                while (ss.poll(timeout, Poco::Net::Socket::SELECT_READ) == false)
                {
                    if (cnt >= 5) {
                        flag_return = 1;
                        qDebug() << "No response from server...";
                        break;
                    }
                    qDebug() << "Waiting to receive data...";
                    cnt++;
                }
            }
            else if(flag_return == 2)
            {
                qDebug() << "=========== Please enter the packet ============";
                qDebug() << "Packet scheme\n| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |";
                qDebug() << "0 : Motion number, 1~6 : Desired data, 7 : Null";
                qDebug() << "================================================";

                try {

                    data.float6dArr[0] = 302.0;
                    data.float6dArr[1] = 0.0;
                    data.float6dArr[2] = 0.0;
                    data.float6dArr[3] = 0.0;
                    data.float6dArr[4] = 0.0;
                    data.float6dArr[5] = 0.0;
                    data.float6dArr[6] = 0.0;
                    data.float6dArr[7] = 0.0;

                    memcpy(writeBuff, data.byte, SIZE_DATA_MAX);
                    if ((int)data.float6dArr[0] < 100)
                    {
                        throw data.float6dArr;
                    }
                    ss.sendBytes(writeBuff, 1024);
                }
                catch (int expn) {
                    qDebug() << "[ERROR] : Please check the Motion";
                }

                if (data.float6dArr[6] < 1.5 && (int)data.float6dArr[0] == 106)
                    qDebug() << "[WARNING] : Please enter a trajectory time longer than 1.5s";

                if ((int)data.float6dArr[0] == 101 && motion_buf == 101)
                {
                    qDebug() << "[WARNING] : Homming has already done";
                }
                motion_buf = (int)data.float6dArr[0];

                flag_return = 1;
            }

            rpt_cnt += 1;

            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();

            emit valueChanged(QString::number(i));
        }
    }
}

void Worker::doMethod3()
{
    qDebug()<<"Starting Method3 in Thread "<<thread()->currentThreadId();

    flag_return=0;
    rpt_cnt=0;
    int i=3;
    while(1)
    {
        mutex.lock();
        bool abort = _abort;
        bool interrupt = _interrupt;
        mutex.unlock();

        if (abort || interrupt) {
            qDebug() << "Interrupting Method3 in Thread "<< thread()->currentThreadId();
            break;
        }
        else
        {
            qDebug() << "Thread Counter : " << rpt_cnt << "sec";

            if(flag_return == 0)
            {
                qDebug() << "=========== Please enter the packet ============";
                qDebug() << "Packet scheme\n| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |";
                qDebug() << "0 : Motion number, 1~6 : Desired data, 7 : Null";
                qDebug() << "================================================";

                try {

                    data.float6dArr[0] = 106.0;
                    data.float6dArr[1] = 0.0;
                    data.float6dArr[2] = -1.57;
                    data.float6dArr[3] = 0.0;
                    data.float6dArr[4] = 0.0;
                    data.float6dArr[5] = 0.0;
                    data.float6dArr[6] = 1.5;
                    data.float6dArr[7] = 0.0;

                    memcpy(writeBuff, data.byte, SIZE_DATA_MAX);
                    if ((int)data.float6dArr[0] < 100)
                    {
                        throw data.float6dArr;
                    }
                    ss.sendBytes(writeBuff, 1024);
                }
                catch (int expn) {
                    qDebug() << "[ERROR] : Please check the Motion";
                }

                if (data.float6dArr[6] < 1.5 && (int)data.float6dArr[0] == 106)
                    qDebug() << "[WARNING] : Please enter a trajectory time longer than 1.5s";

                if ((int)data.float6dArr[0] == 101 && motion_buf == 101)
                {
                    qDebug() << "[WARNING] : Homming has already done";
                }
                motion_buf = (int)data.float6dArr[0];

                flag_return = 2;

            }
            else if(flag_return == 1)
            {
                Timespan timeout(0, 100);
                cnt=0;
                while (ss.poll(timeout, Poco::Net::Socket::SELECT_READ) == false)
                {
                    if (cnt >= 5) {
                        flag_return = 1;
                        qDebug() << "No response from server...";
                        break;
                    }
                    qDebug() << "Waiting to receive data...";
                    cnt++;
                }
            }
            else if(flag_return == 2)
            {
                qDebug() << "=========== Please enter the packet ============";
                qDebug() << "Packet scheme\n| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 |";
                qDebug() << "0 : Motion number, 1~6 : Desired data, 7 : Null";
                qDebug() << "================================================";

                try {

                    data.float6dArr[0] = 301.0;
                    data.float6dArr[1] = 0.0;
                    data.float6dArr[2] = 0.0;
                    data.float6dArr[3] = 0.0;
                    data.float6dArr[4] = 0.0;
                    data.float6dArr[5] = 0.0;
                    data.float6dArr[6] = 0.0;
                    data.float6dArr[7] = 0.0;

                    memcpy(writeBuff, data.byte, SIZE_DATA_MAX);
                    if ((int)data.float6dArr[0] < 100)
                    {
                        throw data.float6dArr;
                    }
                    ss.sendBytes(writeBuff, 1024);
                }
                catch (int expn) {
                    qDebug() << "[ERROR] : Please check the Motion";
                }

                if (data.float6dArr[6] < 1.5 && (int)data.float6dArr[0] == 106)
                    qDebug() << "[WARNING] : Please enter a trajectory time longer than 1.5s";

                if ((int)data.float6dArr[0] == 101 && motion_buf == 101)
                {
                    qDebug() << "[WARNING] : Homming has already done";
                }
                motion_buf = (int)data.float6dArr[0];

                flag_return = 1;
            }

            rpt_cnt += 1;

            QEventLoop loop;
            QTimer::singleShot(1000, &loop, SLOT(quit()));
            loop.exec();

            emit valueChanged(QString::number(i));
        }
    }
}

void Worker::mainLoop()
{
    qDebug()<<"Starting worker mainLoop in Thread "<<thread()->currentThreadId();

    forever {

        mutex.lock();
        if (!_interrupt && !_abort) {
            condition.wait(&mutex);
        }
        _interrupt = false;

        if (_abort) {
            qDebug()<<"Aborting worker mainLoop in Thread "<<thread()->currentThreadId();
            mutex.unlock();
            emit finished();
            return;
        }

        Method method = _method;
        mutex.unlock();

        switch(method) {
        case Method1:
            doMethod1();
            break;
        case Method2:
            doMethod2();
            break;
        case Method3:
            doMethod3();
            break;
        }
    }
}
