#include "onlinestatus.h"
#include <QMap>
#include "user.h"
QMap<QString,bool>onLine;
void initOnlineStatus(){
    for(int i=0;i<userInfo.size();i++){
        onLine[userInfo[i].account]=false;
    }
}
