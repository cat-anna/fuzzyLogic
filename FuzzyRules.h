#pragma once

const static char *ParamNamesTable[] = {
	"TempWew",
	"TempZew",
	"Klima",
	0
};

FuzzyWorld* CreateWorld() {
	FuzzyWorld *fw = new FuzzyWorld();

	fw->NewParameter(ParamNamesTable[0])//wew
		.add("bniski",		Adhesion::Trapezoid::raising(15, 15, 16, 18))
		.add("niski",		Adhesion::Trapezoid::raising(17, 19, 20, 21))
		.add("ok",			Adhesion::Trapezoid::raising(19, 21, 24, 26))
		.add("wysoki",		Adhesion::Trapezoid::raising(24, 26, 30, 33))
		.add("bwysoki",		Adhesion::Trapezoid::raising(25, 30, 40, 40));

	fw->NewParameter(ParamNamesTable[1])//zew
		.add("bniski",		Adhesion::Trapezoid::raising(-10, -10, 0, 10))
		.add("niski",		Adhesion::Trapezoid::raising(0, 10, 17, 22))
		.add("ok",			Adhesion::Trapezoid::raising(19, 20, 25, 26))
		.add("wysoki",		Adhesion::Trapezoid::raising(27, 30, 33, 35))
		.add("bwysoki",		Adhesion::Trapezoid::raising(33, 35, 50, 51));

	fw->NewParameter(ParamNamesTable[2])
		.add("bch³odz",		Adhesion::Trapezoid::raising(-1, -1, -0.85, -0.7))//aby zrównowa¿yæ trapez do ca³kowania
		.add("chlodz",		Adhesion::Trapezoid::raising(-0.75, -0.5, -0.25, 0))
		.add("wy³¹czony",	Adhesion::Trapezoid::raising(-0.5, -0.25, 0.25, 0.5))
		.add("grzanie",		Adhesion::Trapezoid::raising(0, 0.25, 0.5, 0.75))
		.add("bgrzanie",	Adhesion::Trapezoid::raising(0.7, 0.85, 1, 1));//aby zrównowa¿yæ trapez do ca³kowania

	auto &br = fw->GetRules();

	br.add((new FuzzyAnd(fw))->add("TempWew bniski")->add("TempZew bniski")->result("Klima bgrzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew bniski")->add("TempZew niski")->result("Klima bgrzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew bniski")->add("TempZew ok")->result("Klima bgrzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew bniski")->add("TempZew wysoki")->result("Klima grzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew bniski")->add("TempZew bwysoki")->result("Klima grzanie"));

	br.add((new FuzzyAnd(fw))->add("TempWew niski")->add("TempZew bniski")->result("Klima bgrzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew niski")->add("TempZew niski")->result("Klima bgrzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew niski")->add("TempZew ok")->result("Klima grzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew niski")->add("TempZew wysoki")->result("Klima wy³¹czony"));
	br.add((new FuzzyAnd(fw))->add("TempWew niski")->add("TempZew bwysoki")->result("Klima wy³¹czony"));

	br.add((new FuzzyAnd(fw))->add("TempWew ok")->add("TempZew bniski")->result("Klima bgrzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew ok")->add("TempZew niski")->result("Klima grzanie"));
	br.add((new FuzzyAnd(fw))->add("TempWew ok")->add("TempZew ok")->result("Klima wy³¹czony"));
	br.add((new FuzzyAnd(fw))->add("TempWew ok")->add("TempZew wysoki")->result("Klima chlodz"));
	br.add((new FuzzyAnd(fw))->add("TempWew ok")->add("TempZew bwysoki")->result("Klima bch³odz"));

	br.add((new FuzzyAnd(fw))->add("TempWew wysoki")->add("TempZew bniski")->result("Klima wy³¹czony"));
	br.add((new FuzzyAnd(fw))->add("TempWew wysoki")->add("TempZew niski")->result("Klima wy³¹czony"));
	br.add((new FuzzyAnd(fw))->add("TempWew wysoki")->add("TempZew ok")->result("Klima chlodz"));
	br.add((new FuzzyAnd(fw))->add("TempWew wysoki")->add("TempZew wysoki")->result("Klima chlodz"));
	br.add((new FuzzyAnd(fw))->add("TempWew wysoki")->add("TempZew bwysoki")->result("Klima bch³odz"));

	br.add((new FuzzyAnd(fw))->add("TempWew bwysoki")->add("TempZew bniski")->result("Klima wy³¹czony"));
	br.add((new FuzzyAnd(fw))->add("TempWew bwysoki")->add("TempZew niski")->result("Klima wy³¹czony"));
	br.add((new FuzzyAnd(fw))->add("TempWew bwysoki")->add("TempZew ok")->result("Klima chlodz"));
	br.add((new FuzzyAnd(fw))->add("TempWew bwysoki")->add("TempZew wysoki")->result("Klima bch³odz"));
	br.add((new FuzzyAnd(fw))->add("TempWew bwysoki")->add("TempZew bwysoki")->result("Klima bch³odz"));

	return fw;
}