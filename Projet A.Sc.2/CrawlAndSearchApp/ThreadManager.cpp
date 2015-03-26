#include "stdafx.h"
#include "ThreadManager.h"

namespace System
{

#pragma region Constructors

	ThreadManager::ThreadManager()
	{
		runningCount = 0;
		maxThread = 2;
		if (concurentThreadsSupported > 0)
			maxThread = concurentThreadsSupported;
		SetTimeout(30);
	}

	ThreadManager::~ThreadManager()
	{
		importantTasksQueue.clear();
		normalTasksQueue.clear();
	}

#pragma endregion

#pragma region Methods

	void ThreadManager::DoTasks(TaskType taskType)
	{
		if (taskType == IMPORTANT)
		{
			if (importantTasksWorking)
				return;
			importantTasksWorking = true;
		}
		else
		{
			if (normalTasksWorking)
				return;
			normalTasksWorking = true;
		}

		thread doTasksAsyncThread(&ThreadManager::DoTasksAsync, this, taskType);
		doTasksAsyncThread.detach();
	}

	void ThreadManager::DoTasksAsync(TaskType& taskType)
	{
		map<long, function<void()>> &tasksQueue = (taskType == IMPORTANT) ? importantTasksQueue : normalTasksQueue;
		vector<Task> tasks;
		int count = tasksQueue.size();

		tasks.reserve(maxThread);

		while (count > 0)
		{
			int to = 0;
			int i = 0;
			map<long, function<void()>>::iterator iteratorQueue;
			auto iteratorTasks = tasks.begin();

			if (tasks.size() > 0)
			{
				bool exit = false;
				while (!exit && iteratorTasks != tasks.end()) // Supprime les tâches terminées
				{
					if (iteratorTasks->IsFinished())
					{
						tasksQueue.erase(iteratorTasks->GetId());
						iteratorTasks = tasks.erase(iteratorTasks);
						runningCount--;
					}
					else
						++iteratorTasks;
				}
			}

			count = tasksQueue.size();
			if (count > maxThread)
				to = maxThread - tasks.size();
			else
				to = count - tasks.size();

			iteratorQueue = tasksQueue.begin();

			if ((taskType == IMPORTANT || importantTasksQueue.size() == 0) && to > 0) // Si on est cherche à exécuter des tâches normales et qu'il reste des tâches importantes, on ne fait rien.
			{
				while (i < to && iteratorQueue != tasksQueue.end())
				{
					bool runThisTask = true;
					iteratorTasks = tasks.begin();

					i++;

					if (tasks.size() > 0)
					{
						while (runThisTask && iteratorTasks != tasks.end()) // Vérifie si la tâhces est pas déja en cours.
						{
							if (iteratorTasks->GetId() == iteratorQueue->first)
								runThisTask = false;
							++iteratorTasks;
						}
					}

					if (runThisTask)
					{
						runningCount++;
						tasks.emplace_back(Task(iteratorQueue->first, iteratorQueue->second, GetTimeout()));
					}
					else
						i--;
					++iteratorQueue;
				}
			}
		}

		tasks.clear();
		runningCount = 0;

		if (taskType == IMPORTANT)
			importantTasksWorking = false;
		else
			normalTasksWorking = false;
	}

#pragma endregion

#pragma region Getters & Setters

	int	ThreadManager::GetRunningCount()
	{
		return runningCount;
	}

	void ThreadManager::SetTimeout(int seconds)
	{
		timeout = seconds * 1000;
	}

	int	ThreadManager::GetTimeout()
	{
		return timeout / 1000;
	}

#pragma endregion

}