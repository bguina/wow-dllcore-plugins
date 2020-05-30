#pragma once

#include <string>
#include <utility>
#include <map>

#include "ABenBot.h"

class IPathFinder;



//template<typename State, typename Trigger>
class ABenSMBot : public ABenBot
{
public:
	enum class State {
		Idle,
		WaitingForMoney,
		FindCan
	};

	enum class Message {
		MakeChoice,
		Cancel,
		MoneyEntered,
		CanSentToUser
	};


	enum class Trigger {
		WaitingForMoney,
		Idle,
		FindCan
	};


	ABenSMBot(WowGame& game, const std::string& tag) : ABenBot(game, tag){

		/*
		InitialState = Looking for "objective"
		-> Disconnected / Connected
		-> Evaluate required conditions to complete objective
		-> In danger / objective in reach / objective triggered / far from objective



		EBotObjective
			- gain experience
			- kill specific enemies
			- stay alive

			baesien logic? evaluation bias  mk

		Disonnected -> Define objective, maybe logout
		InGame		-> GameStateMachine

		*/

		mDiagram.insert(std::make_pair(State::Idle, Message::MakeChoice), State::WaitingForMoney);
		mDiagram.insert(std::make_pair(State::WaitingForMoney, Message::Cancel), State::Idle);
		mDiagram.insert(std::make_pair(State::WaitingForMoney, Message::MoneyEntered), State::FindCan);
		mDiagram.insert(std::make_pair(State::FindCan, Message::CanSentToUser), State::Idle);

	}
	virtual ~ABenSMBot();

	virtual void getEvaluation() const;

	virtual void takeAction();

	virtual void onEvaluate() final override {

	}

protected:


private:
	typedef std::pair<State, Trigger> PairStateTrigger;
	typedef std::map<PairStateTrigger, State> StateDiagram;

	typedef std::map<State, State> StateDiagramForDefaults;

	StateDiagram mDiagram;

	virtual void _logDebug() const override;
};

inline std::ostream& operator<<(std::ostream& out,const class ABenBot& obj) {
	out << dynamic_cast<const BaseWowBot&>(obj);
	return out;
}
