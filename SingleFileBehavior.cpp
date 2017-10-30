#include "SingleFileBehavior.h"
#include "ParamLoader.h"
#include "SteeringBehaviors.h"

// TODO: Maybe add an entity type in BaseGameEntity, and check it in
// Update of Follower.
Leader::Leader(GameWorld* world, Vector2D& initialPos)  : Vehicle(world,
																  initialPos,                 //initial position
																  RandFloat()*TwoPi,        //start rotation
																  Vector2D(0, 0),            //velocity
																  Prm.VehicleMass,          //mass
																  Prm.MaxSteeringForce,     //max force
																  Prm.MaxSpeed,             //max velocity
																  Prm.MaxTurnRatePerSecond, //max turn rate
																  Prm.VehicleScale * 5,
																  true)
{
	Steering()->WanderOn();
}

void Leader::toggleMindControl() {
	if (m_IsMindControlled) {
		m_IsMindControlled = false;
		Steering()->WanderOn();
	}
	else {
		m_IsMindControlled = true;
		Steering()->WanderOff();
		this->SetVelocity(Vector2D(0, 0));
	}
}

void Leader::manualDirection(Vector2D v) {
	int const offset = 100;
	if (m_IsSetInMotion && m_IsMindControlled) {
		Steering()->OffsetPursuitOn(this, Vector2D(v.x * offset, v.y * offset));
	}
}

void Leader::setIsSetInMotion(bool b) {
	m_IsSetInMotion = b;
	if (b == false)
		this->SetVelocity(Vector2D(0, 0));
	else
		this->SetVelocity(Vector2D(this->m_vHeading.x * this->m_dMaxSpeed, this->m_vHeading.y * this->m_dMaxSpeed));
}

void  Leader::resetSpeed() {
	this->SetVelocity(Vector2D(this->m_vVelocity.x * this->m_dMaxSpeed, this->m_vVelocity.y * this->m_dMaxSpeed));
}

Follower::Follower(GameWorld* world, Vector2D& initialPos, bool followLeader) : Vehicle(world,
																						 initialPos,                 //initial position
																						 RandFloat()*TwoPi,        //start rotation
																						 Vector2D(0, 0),            //velocity
																						 Prm.VehicleMass,          //mass
																						 Prm.MaxSteeringForce,     //max force
																						 Prm.MaxSpeed,             //max velocity
																						 Prm.MaxTurnRatePerSecond, //max turn rate
																						 Prm.VehicleScale)        //scale
{
	m_IsFollowingLeader = followLeader;

	if (!m_IsFollowingLeader) {
		Vehicle* target = World()->Agents().back();
		SetTarget(target);
	}
}

void Follower::SetTarget(Vehicle* target) {
	if (target) {
		Steering()->OffsetPursuitOn(target, Vector2D(Prm.AgentOffsetX, Prm.AgentOffsetY));
	}
}

void Follower::Update(double time_elapsed) {
	if (m_IsFollowingLeader) {
		auto& allAgents = World()->Agents();
		double minDistanceSq = -1;
		Vehicle *leader = NULL;

		for (int i = 0; i < Prm.NumLeaders; ++i) {
			Vehicle *agent = allAgents[i];

			// Make sure we are actually looking at a leader.
			assert(dynamic_cast<Leader*>(agent));
				
			double distanceSq = (agent->Pos() - Pos()).LengthSq();
			
			if ((minDistanceSq == -1) ||
				(distanceSq < minDistanceSq)) {
				leader = agent;
				minDistanceSq = distanceSq;
			}
		}

		SetTarget(leader);
	}

	Vehicle::Update(time_elapsed);
}
