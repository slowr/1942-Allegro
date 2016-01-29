#pragma once

#include <list>
#include <algorithm>
#include "Animator.h"
#include <iostream>

typedef std::list<Animator*> AnimatorList;

class AnimatorHolder {
	static AnimatorList running, suspended, dead;
	class ProgressFunctor : public std::unary_function<Animator*, void> {
			timestamp_t t;
		public:
			void operator()(Animator* a) const;
			ProgressFunctor(timestamp_t _t);
	};
public:
	static void Register(Animator* a);
	static void Cancel(Animator* a);
	static void MarkAsRunning(Animator* a);
	static void MarkAsSuspended(Animator* a);
	static void MarkAsDead(Animator *a);
	static void Progress(timestamp_t currTime);
};
