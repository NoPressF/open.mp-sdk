#pragma once

#include <anim.hpp>
#include <component.hpp>
#include <player.hpp>
#include <types.hpp>
#include <values.hpp>

struct ActorSpawnData
{
	Vector3 position;
	float facingAngle;
	int skin;
};

/// Actor base interace
struct IBaseActor : public IExtensible, public IEntity
{
	/// Sets the actor's skin
	virtual void setSkin(int id) = 0;

	/// Gets the actor's model
	virtual int getSkin() const = 0;

	/// Apply an animation for the actor
	virtual void applyAnimation(const AnimationData& animation) = 0;

	/// Get the actor's applied animation
	virtual const AnimationData& getAnimation() const = 0;

	/// Clear the actor's animations
	virtual void clearAnimations() = 0;

	/// Set the actor's health
	virtual void setHealth(float health) = 0;

	/// Get the actor's health
	virtual float getHealth() const = 0;

	/// Set whether the actor is invulnerable
	virtual void setInvulnerable(bool invuln) = 0;

	/// Get whether the actor is invulnerable
	virtual bool isInvulnerable() const = 0;

	/// Checks if actor is streamed for a player
	virtual bool isStreamedInForPlayer(const IPlayer& player) const = 0;

	/// Streams actor for a player
	virtual void streamInForPlayer(IPlayer& player) = 0;

	/// Streams out actor for a player
	virtual void streamOutForPlayer(IPlayer& player) = 0;

	/// Set actor state hidden or shown for a player (only process streaming if actor is not hidden)
	virtual void setActorHiddenForPlayer(IPlayer& player, bool hidden) = 0;

	/// Check if given actor has hidden state for player (only process streaming if actor is not hidden)
	virtual bool isActorHiddenForPlayer(IPlayer& player) const = 0;

	/// Get actor spawn data
	virtual const ActorSpawnData& getSpawnData() = 0;
	
	/// Used by legacy per-player actors for ID mapping.
	virtual void setLegacyPlayer(IPlayer* player) = 0;

	/// Used by legacy per-player actors for ID mapping.
	virtual IPlayer* getLegacyPlayer() const = 0;
};

struct IActor : public IBaseActor
{
};

struct IPlayerActor : public IBaseActor
{
};

struct ActorEventHandler
{
	virtual void onPlayerGiveDamageActor(IPlayer& player, IActor& actor, float amount, unsigned weapon, BodyPart part) { }
	virtual void onActorStreamOut(IActor& actor, IPlayer& forPlayer) { }
	virtual void onActorStreamIn(IActor& actor, IPlayer& forPlayer) { }
};

static const UID ActorsComponent_UID = UID(0xc81ca021eae2ad5c);
struct IActorsComponent : public IPoolComponent<IActor>
{
	PROVIDE_UID(ActorsComponent_UID);

	/// Get the ActorEventHandler event dispatcher
	virtual IEventDispatcher<ActorEventHandler>& getEventDispatcher() = 0;

	/// Create an actor
	virtual IActor* create(int skin, Vector3 pos, float angle) = 0;

	/// Get the ID of this actor as used in old pools (i.e. in pawn).
	virtual int toLegacyID(int real) const = 0;

	/// Get the ID of this actor as used in the SDK.
	virtual int fromLegacyID(int legacy) const = 0;

	/// Release the ID used in limited pools.
	virtual void releaseLegacyID(int legacy) = 0;

	/// Return an ID not yet used in pawn (et al) to represent this actor.
	virtual int reserveLegacyID() = 0;

	/// Assign a full ID to the legacy ID reserved earlier.
	virtual void setLegacyID(int legacy, int real) = 0;
};

static const UID ActorsData_UID = UID(0x6640545364455261);
struct IPlayerActorData : public IExtension
{
	PROVIDE_EXT_UID(ActorsData_UID);

	/// Get the ID of this actor as used in old pools (i.e. in pawn).
	virtual int toLegacyID(int real) const = 0;

	/// Get the ID of this actor as used in the SDK.
	virtual int fromLegacyID(int legacy) const = 0;

	/// Release the ID used in limited pools.
	virtual void releaseLegacyID(int legacy) = 0;

	/// Return an ID not yet used in pawn (et al) to represent this actor.
	virtual int reserveLegacyID() = 0;

	/// Assign a full ID to the legacy ID reserved earlier.
	virtual void setLegacyID(int legacy, int real) = 0;

	/// Get the ID of this actor as used internally (i.e. sent to the client).
	virtual int toClientID(int real) const = 0;

	/// Get the ID of this actor as used in the SDK.
	virtual int fromClientID(int legacy) const = 0;

	/// Release the ID used on the client.
	virtual void releaseClientID(int legacy) = 0;

	/// Return an ID not yet used on the client to represent this actor.
	virtual int reserveClientID() = 0;

	/// Assign a full ID to the legacy ID reserved earlier.
	virtual void setClientID(int legacy, int real) = 0;
};
