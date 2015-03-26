#pragma once

#include <thread>
#include <ppltasks.h>
#include <map>
#include <mutex>
#include <limits.h>

#include "Task.h"
#include "VectorExtension.h"

using namespace std;
using namespace Concurrency;

namespace System
{
	class ThreadManager
	{
	public:
		enum TaskType { NORMAL, IMPORTANT }; // Se sont des priorités : tasks IMPORTANT prioritaire sur les tasks NORMAL

		ThreadManager();
		~ThreadManager();

		template<class Callback, class... Args>
		void AddTaskToRun(ThreadManager::TaskType taskType, Callback&& fun, Args&&... args);

		void DoTasks(TaskType taskType = IMPORTANT);

		int	 GetRunningCount();

		void SetTimeout(int seconds);
		int	 GetTimeout();

	private:
		unsigned						concurentThreadsSupported = thread::hardware_concurrency(); // retourne le nombre de coeurs virtuelles de la machine. Retourne 0 si ce n'est pas supporté. Par défaut, la valeur est 2 (voir constructeur)
		long							taskCount = 0;
		int								maxThread;
		int								runningCount;
		int								timeout;
		bool							importantTasksWorking;
		bool							normalTasksWorking;

		map<long, function<void()>>		importantTasksQueue;
		map<long, function<void()>>		normalTasksQueue;
		mutex							threadMutex; // gestion du thread safe

		void							DoTasksAsync(TaskType& taskType);
	};

	template<class Callback, class... Args> // Ce template permet de faire en sorte de donnér en paramètre un référence à une fonction, suivit de ses N arguments.
	void ThreadManager::AddTaskToRun(ThreadManager::TaskType taskType, Callback&& func, Args&&... args)
	{
		taskCount++;

		function<void(void)> task = bind(_Decay_copy(forward<Callback>(func)), _Decay_copy(forward<Args>(args))...); // On créer une tâche (non exécuté pour le moment). _Decay_copy effectue une copie des valeurs des arguments, afin de les conserver dans le temps.

		threadMutex.lock();
		if (taskType == IMPORTANT) // Et on ajoute la tâche à la file d'attente.
			importantTasksQueue.insert(importantTasksQueue.end(), make_pair(taskCount, task));
		else
			normalTasksQueue.insert(normalTasksQueue.end(), make_pair(taskCount, task));
		threadMutex.unlock();

		if (taskCount == 2147483646) // Max long = 2147483647
			taskCount = 0;

		DoTasks(taskType);
	}
}