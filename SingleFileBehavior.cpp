#include "SingleFileBehavior.h"
#include "ParamLoader.h"
#include "SteeringBehaviors.h"

// TODO: Maybe add an entity type in BaseGameEntity, and check it in
// Update of Follower.
Leader::Leader(GameWorld* world, Vector2D& initialPos)  : m_IsMindControlled(false),
														  Vehicle(world,
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

Follower::Follower(GameWorld* world, Vector2D& initialPos) : Vehicle(world,
																	 initialPos,                 //initial position
																	 RandFloat()*TwoPi,        //start rotation
																	 Vector2D(0, 0),            //velocity
																	 Prm.VehicleMass,          //mass
																	 Prm.MaxSteeringForce,     //max force
																	 Prm.MaxSpeed,             //max velocity
																	 Prm.MaxTurnRatePerSecond, //max turn rate
																	 Prm.VehicleScale)        //scale
{
	static bool isFirst = true;

	m_IsFollowingLeader = isFirst;
	isFirst = false;

	if (!m_IsFollowingLeader) {
		Vehicle* target = World()->Agents().back();
		SetTarget(target);
	}
}

void Follower::SetTarget(Vehicle* target) {
	if (target) {
		Steering()->OffsetPursuitOn(target, Vector2D(0.5, 0.5));
	}
}

void Follower::Update(double time_elapsed) {
	if (m_IsFollowingLeader) {
		auto& allAgents = World()->Agents();
		double minDistanceSq = -1;
		Vehicle *leader = NULL;

		for (int i = 0; i < Prm.NumLeaders; ++i) {
			Vehicle *agent = allAgents[i];

			// Make sure we are actually looking at a Leader agent.
			assert(dynamic_cast<Leader *>(agent));
				
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
