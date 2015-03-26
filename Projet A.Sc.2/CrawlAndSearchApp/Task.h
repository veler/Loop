#pragma once

#include "stdafx.h"
#include <ppltasks.h>
#include <agents.h>
#include <iostream>

using namespace std;
using namespace Concurrency;

namespace System
{
	class Task
	{
	public:
		Task(const Task& other);
		Task(long taskId, function<void(void)> func, int timeoutSeconds);
		bool	IsFinished();
		long	GetId();

		void	SetTimeout(int seconds);
		int		GetTimeout();

		Task&	operator=(Task other);

	private:
		cancellation_token_source	cts;
		task<void>					taskToRun;
		long						id;
		int							timeout;

		template<typename T>
		task<T>						CancelAfterTimeout(task<T> t, cancellation_token_source cts);
		task<void>					CompleteAfter();
	};
}
