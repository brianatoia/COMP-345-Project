#pragma once
#include <list>
#include <iostream>
#include <memory>
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
	void notify();
};

class Observer
{
	Subject* model;
public:
	Observer(Subject* model);
	~Observer();

	virtual void update() = 0;
	virtual void show() = 0;

protected:
	Subject* getSubject() { return model; };
};

class PhaseObserver : public Observer
{
public:
	PhaseObserver(Subject* model);
	~PhaseObserver();

	void update();
	void show();
};

class StatisticsObserver : public Observer
{
	shared_ptr<Map> map;
	vector<shared_ptr<Player>> players;

public:
	StatisticsObserver(Subject* model, shared_ptr<Map> map, vector<shared_ptr<Player>> players);
	~StatisticsObserver();

	void update();
	void show();
};