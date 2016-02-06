#pragma once

#include <list>
#include <algorithm>
#include "Animator.h"
#include <iostream>

typedef std::list<Animator*> AnimatorList;

class AnimatorHolder {
	static AnimatorList running, suspended;
	class ProgressFunctor : public std::unary_function<Animator*, void> {
		public:
			void operator()(Animator* a) const;
			ProgressFunctor();
	};
public:
	static void Register(Animator* a);
	static void Cancel(Animator* a);
	static void MarkAsRunning(Animator* a);
	static void MarkAsSuspended(Animator* a);
	static void Progress();
};
