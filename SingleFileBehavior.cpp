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

	Steering()->SeparationOn();
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
