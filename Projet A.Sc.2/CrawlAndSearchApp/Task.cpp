#include "stdafx.h"
#include "Task.h"

namespace System
{

#pragma region Constructors

	Task::Task(const Task& other)
	{
		cts = other.cts;
		taskToRun = other.taskToRun;
		id = other.id;
		timeout = other.timeout;
	}

	Task::Task(long taskId, function<void(void)> func, int timeoutSeconds)
	{
		cancellation_token cancelToken = cts.get_token();

		SetTimeout(timeoutSeconds);
		id = taskId;

		taskToRun = CancelAfterTimeout(create_task([=](void)
		{
			func();
		}, cancelToken), cts).then([this, cancelToken]()
		{
			if (cancelToken.is_canceled())
				cancel_current_task();
		}, cancelToken);
	}

#pragma endregion

#pragma region Operators

	Task& Task::operator=(Task other)
	{
		cts = other.cts;
		taskToRun = other.taskToRun;
		id = other.id;
		timeout = other.timeout;
		return *this;
	}

#pragma endregion

#pragma region Functions

	bool Task::IsFinished()
	{
		return taskToRun.is_done();
	}

	template<typename T>
	task<T> Task::CancelAfterTimeout(task<T> taskToRun, cancellation_token_source cts)
	{
		task<bool> success_task = taskToRun.then([](T)
		{
			return true;
		});
		task<bool> failure_task = CompleteAfter().then([]
		{
			return false;
		});

		return (failure_task || success_task).then([taskToRun, cts](bool success)
		{
			if (!success)
				cts.cancel();
			return taskToRun;
		});
	}

	task<void> Task::CompleteAfter()
	{
		task_completion_event<void> tce;

		auto fire_once = new timer<int>(timeout, 0, nullptr, false);
		auto callback = new call<int>([tce](int)
		{
			tce.set();
		});

		fire_once->link_target(callback);
		fire_once->start();

		task<void> event_set(tce);

		return event_set.then([callback, fire_once]()
		{
			delete callback;
			delete fire_once;
		});
	}

#pragma endregion

#pragma region Getters & Setters

	long Task::GetId()
	{
		return id;
	}

	void Task::SetTimeout(int seconds)
	{
		timeout = seconds * 1000;
	}

	int	Task::GetTimeout()
	{
		return timeout / 1000;
	}

#pragma endregion

}