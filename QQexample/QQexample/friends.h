#ifndef FRIENDS_H
#define FRIENDS_H
#include <QString>
const int Max_user=100;
extern int friendShip[Max_user][Max_user];
void initFriend(const QString& xmlPath );
void setFriendShip(int id1,int id2,bool areFriends);
void setNewFriendShip(int id1,int id2,bool areFriends);
//void reSetFriendShip(int id1,int id2,const QString& xmlPath,bool areFriends);
#endif // FRIENDS_H
