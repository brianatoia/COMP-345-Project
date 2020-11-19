#pragma once
#include <list>
#include <iostream>
#include <memory>
#include <string>
#include <windows.h>
#include "../Map/Map.h"
#include "../Player/Player.h"

using namespace std;

// GameEngine is a subject

class Subject
{
	list<class Observer*> *observers;
public:
	Subject();
	~Subject();
	void attach(class Observer* observer);
	void detach(class Observer* observer);
	void notify(string message);
};

class Observer
{
	Subject* model;
public:
	Observer(Subject* model);
	~Observer();

	virtual void update(string message) = 0;

	vector<string> tokenize(string);

protected:
	Subject* getSubject() { return model; };
};

class PhaseObserver : public Observer
{
	vector<shared_ptr<Player>> players;

public:
	PhaseObserver(Subject* model, vector<shared_ptr<Player>> players);
	~PhaseObserver();

	void update(string message);
};

class StatisticsObserver : public Observer
{
	shared_ptr<Map> map;
	vector<shared_ptr<Player>> players;

public:
	StatisticsObserver(Subject* model, shared_ptr<Map> map, vector<shared_ptr<Player>> players);
	~StatisticsObserver();

	void update(string message);
	void show();
};