#ifndef _SINGLEFILEBEHAVIOR_H_
#define _SINGLEFILEBEHAVIOR_H_

#include "Vehicle.h"
#include "GameWorld.h"

// NOTE: This implements Leader - Follower behaviour
//
// The follower is supposed to 'follow the agent from its group just
// in front of him'. This is implemented as 'follow the previously
// created agent'.
class Leader : public Vehicle
{	
public:
	bool m_IsMindControlled = false;
	bool m_IsSetInMotion = false;
    Leader(GameWorld* world, Vector2D& initialPos);
	
	void toggleMindControl();
	void manualDirection(Vector2D v);
	void setIsSetInMotion(bool b);
	void resetSpeed();

};


class Follower : public Vehicle
{
private:
	bool m_IsFollowingLeader;
	void SetTarget(Vehicle* target);
	
public:
    Follower(GameWorld* world, Vector2D& initialPos, bool followLeader);
	virtual void Update(double time_elapsed);
};
#endif /* _SINGLEFILEBEHAVIOR_H_ */
