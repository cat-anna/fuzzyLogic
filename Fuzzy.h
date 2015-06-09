/*
* main.cpp
*
*  Created on: 5 lis 2014
*      Author: Paweu
*/

#pragma unmanaged

#include <cstdio>
#include <iostream>
#include <functional>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>

#undef min
#undef max

template <class T> const T & min (const T &t1, const T &t2) { return t1<t2?t1:t2; }
template <class T> const T & max (const T &t1, const T &t2) { return t1>t2?t1:t2; }


enum class FuzzyMathMode{
	Classic,
};

struct FuzzyMath {
	typedef std::vector<float> MathInput;
	virtual float And(const MathInput &d) = 0;
	virtual float Or (const MathInput &d) = 0;
};

struct FuzzyClassicMath : public FuzzyMath {
	float And(const MathInput &d) {
		if(d.empty()) return 0;
		float m = d[0];
		for(unsigned i = 1; i < d.size(); ++i)
			m = min(m, d[i]);
		return m;
	}
	float Or(const MathInput &d) {
		if(d.empty()) return 0;
		float m = d[0];
		for(unsigned i = 1; i < d.size(); ++i)
			m = max(m, d[i]);
		return m;
	}
};

struct FuzzyWord;
struct FuzzyWorld;
struct FuzzyParameter;
struct FuzzyRuleContainer;

struct AdhesionRange {
	float a, b;
	AdhesionRange(float aa = 0.0f, float ab = 0.0f): a(aa), b(ab) { }
};

typedef std::function<float(float)> AdhesionFunctionPtr;
typedef std::function<float(float)> AdhesionRangeFunctionPtr;

struct AdhesionFunctionImpl {
	virtual ~AdhesionFunctionImpl() { };
	virtual float cast(float pos) const = 0;
	virtual float leftMax(float pos) const = 0;
	virtual float rightMax(float pos) const = 0;
	virtual float BeginFor(float pos) const = 0;
	virtual float EndFor(float pos) const = 0;
};

struct AdhesionFunction {
	AdhesionFunction(): m_impl() { }
	AdhesionFunction(AdhesionFunctionImpl *impl): m_impl(impl){ }
	~AdhesionFunction() { }

	float operator()(float f) const {
		return m_impl->cast(f);
	};

	const AdhesionFunctionImpl* operator->() const { return m_impl.get(); }
protected:
	std::shared_ptr<AdhesionFunctionImpl> m_impl;
};

struct Adhesion {
	struct Linear{
		template <bool REV>
		struct LinearImpl  : public AdhesionFunctionImpl {
			float cast(float f) const {
				if(REV)
					return 1.0f - process(f);
				else
					return process(f);
			}
			float leftMax(float f) const {
				if(REV)
					return reverse(1.0f - f);
				else
					return reverse(f);
			}
			float rightMax(float f) const { return leftMax(1.0f - f); }
			float BeginFor(float pos) const { return start; }
			float EndFor(float pos) const { return end; }
			LinearImpl(float s, float e): start(s), end(e) { }
		protected:
			float start, end;
			float process(float f) const {
				if(f <= start) return 1;
				if(f >= end) return 0;
				return (end - f) / (end - start);
			}
			float reverse(float f) const {
				if(f <= 0) return start;
				if(f >= 1) return end;
				return (end - start) * f + start;
			}
		};

		static AdhesionFunction falling(float start, float end) {
			return AdhesionFunction(new LinearImpl<false>(start, end));
		}
		static AdhesionFunction raising(float start, float end) {
			return AdhesionFunction(new LinearImpl<true>(start, end));
		}
	};

	struct Triangle {
		template <bool REV>
		struct TriangleImpl  : public AdhesionFunctionImpl {
			float cast(float f) const {
				if(REV)
					return 1.0f - process(f);
				else
					return process(f);
			}
			float leftMax(float f) const {
				if(!REV)
					return reverse(1.0f - f);
				else
					return reverse(f);
			}
			float BeginFor(float pos) const { return left; }
			float EndFor(float pos) const { return right; }
			float rightMax(float f) const { return leftMax(1.0f - f); }
			TriangleImpl(float l, float c, float r): left(l), center(c), right(r) { }
		protected:
			float left, center, end, right;
			float process(float f) const {
				if(f < left || f > right) return 0;
				if(f < center)  return (f - left) / (center - left);
				if(f == center) return 1;
				return (right - f) / (right - center);
			}
			float reverse(float f) const {
				if(f <= 0) return left;
				if(f >= 1) return right;
				if(f <= 0.5f)
					return (center - left) * (f * 2.0f) + left;
				return (right - center) * ((f - 0.5f) * 2.0f) + center;
			}
		};

		static AdhesionFunction raising(float left, float center, float right) {
			return AdhesionFunction(new TriangleImpl<false>(left, center, right));

		}
		static AdhesionFunction falling(float left, float center, float right) {
			return AdhesionFunction(new TriangleImpl<true>(left, center, right));
		}
	};

	struct Trapezoid {
		struct TrapezoidImpl : public AdhesionFunctionImpl {
			float cast(float f) const {
					return process(f);
			}
			float leftMax(float f) const {
				if(f <= 0) return points[0];
				if(f >= 1) return points[1];
				return (points[1] - points[0]) * f + points[0];
			}
			float rightMax(float f) const { 
				if(f <= 0) return points[3];
				if(f >= 1) return points[2];
				return points[3] - (points[3] - points[2]) * (1.0f - f);
			}
			float BeginFor(float pos) const { return points[0]; }
			float EndFor(float pos) const { return points[3]; }
			TrapezoidImpl(float a, float b, float c, float d) { 
				points[0] = a; points[1] = b; points[2] = c; points[3] = d;
			}
			float center(float pos) const { return (points[1] + points[2]) / 2.0f; };
		protected:
			float points[4];
			float process(float f) const {
				if(f < points[0] || f > points[3]) return 0;
				if(f < points[1])  return (f - points[0]) / (points[1] - points[0]);
				if(f >= points[1] && f <= points[2]) return 1;
				return (points[3] - f) / (points[3] - points[2]);
			}
		};
		static AdhesionFunction raising(float a, float b, float c, float d){
			return AdhesionFunction(new TrapezoidImpl(a, b, c, d));
		}
	};
};

struct FuzzyWordValue {
	const FuzzyWord *Word;
	float value;
	FuzzyWordValue(FuzzyWord *w = 0, float f = 0): Word(w), value(f) { }

	operator bool() const {
		return value > 0;
	}
};

struct FuzzyWordValueList : protected std::list<FuzzyWordValue> {
	typedef std::list<FuzzyWordValue> Base;
	using Base::begin;
	using Base::end;
	using Base::size;
	using Base::clear;

	const FuzzyWordValue* GetFor(const FuzzyWord* w) const {
		for(auto &it: *this)
			if(it.Word == w)
				return &it;
		return 0;
	};

	void Append(const FuzzyWordValueList& other) {
		for(auto &it: other)
			push_back(it);
	}

	FuzzyWordValue* Add(const FuzzyWordValue &w) { 
		push_back(w); 
		return &back();
	}
};

struct FuzzyWord {
	const FuzzyParameter *Owner;
	std::string Name;
	unsigned Index;
	AdhesionFunction function;

	FuzzyWordValue FromParameter(float value) const {
		FuzzyWordValue f;
		f.Word = this;
		f.value = function(value);
		return f;
	}

};

#include <fstream>

struct FuzzyParameter {
	typedef std::map<std::string, FuzzyWord> WordVector;

	const FuzzyWord* get(const std::string &name) const {
		auto it = m_words.find(name);
		if(it != m_words.end())
			return &(it->second);
		return 0;
	}

	FuzzyParameter& add(const std::string &name, AdhesionFunction func) {
		FuzzyWord f;
		f.Name = name;
		f.Index = m_words.size();
		f.function = func;
		f.Owner = this;
		m_words.insert(std::make_pair(name, f));
		return *this;
	}

	FuzzyWordValueList GetValueOf(float value) const {
		FuzzyWordValueList r;
		for(auto &it: m_words) {
			auto w = it.second.FromParameter(value);
			if(w)
				r.Add(w);
		}
		return r;
	}

	FuzzyParameter(const std::string &name): m_Name(name) { }
	const std::string &GetName() const { return m_Name; }
	const WordVector& AccessWords() const { return m_words; }

	float ReverseProcess(const std::vector<std::pair<const FuzzyWord*, float>> &inp) const {
		if(inp.empty()) return 0;
		bool first = true;
		float a = 0, b = 0, r = 0;
		for(auto &it: inp) {
			if(first || a > it.first->function->BeginFor(0)) a = it.first->function->BeginFor(0);
			if(first || b < it.first->function->EndFor(0)) b = it.first->function->EndFor(0);
			first = false;
		}

		float n = 100;
		float dx = (b - a) / n;
		std::ofstream log("log.txt");
		log << a << "\t" << b << "\t" << dx << std::endl;
		float mx = 0, m = 0;
		for(float x = a; x < b+dx; x += dx) {

			float step = 0;
			bool first = true;
			for(auto &it: inp) {
			//	float t = it.first->function->cast(x);

				if(x <= it.first->function->BeginFor(0)) continue;
				if(x >= it.first->function->EndFor(0)) continue;
				float t = min(it.second, it.first->function->cast(x));
				if(first || step < t) step = t;
				first = false;
			}
			log << x << "\t" << step << std::endl;
			if(first) continue;
			m += step * dx;
			mx += x * step * dx;
		}
		return mx / m;
	/*	bool first = true;
		float r = 0;
		for(auto &it: inp) {
			float t = it.first->function->rightMax(it.second);
			if(first || t > r) r = t;

			first = false;
		}*/

	//	return r;
	}
protected:
	std::string m_Name;
	WordVector m_words;
};

enum class RuleTestResult {
	Passed,
	NotPassed,
	ValueNotExists,
};

struct FuzzyTestResult {
	RuleTestResult result;
	float value;
	const FuzzyWord *word;
	FuzzyTestResult(float f, const FuzzyWord *w): result(RuleTestResult::Passed), value(f), word(w) {}
	FuzzyTestResult(RuleTestResult r): result(r), value(0), word(0) {}
};

struct FuzzyRule {
	virtual ~FuzzyRule() { }
	virtual FuzzyTestResult Test() const = 0;
protected:
	FuzzyWorld *m_world;
	FuzzyRule(FuzzyWorld *w): m_world(w) { }
};


struct FuzzyWorld {
	typedef std::map<std::string, FuzzyParameter> ParameterVector;
	typedef std::map<const FuzzyParameter *, float> ResultContainer;

	void AddParameter(const FuzzyParameter &fp) {
		m_params.insert(std::make_pair(fp.GetName(), fp));
	}
	FuzzyParameter& NewParameter(const std::string &name) {
		AddParameter(FuzzyParameter(name));
		return *(FuzzyParameter*)FindParameter(name);
	}

	const FuzzyParameter* FindParameter(const std::string &Name) const {
		auto it = m_params.find(Name);
		if(it != m_params.end())
			return &(it->second);
		return 0;
	}
	const ParameterVector& AccessParameter() const { return m_params; }

	void AddParameterValue(const std::string& pname, float value) {
		auto *p = FindParameter(pname);
		if(!p) throw "No such parameter!";
		m_values.Append(p->GetValueOf(value));
	}
	const FuzzyWordValueList& AccessValues() const { return m_values; }
	const FuzzyWordValue* ValueOf(const FuzzyWord* w) const {
		return m_values.GetFor(w);
	}
	FuzzyWordValue* PushValue(const FuzzyWord *w, float f) {
		FuzzyWordValue v;
		v.value = f;
		v.Word = w;
		return m_values.Add(v);
	}

	const FuzzyRuleContainer& GetRules() const { return *m_BaseRule; }
	FuzzyRuleContainer& GetRules() { return *m_BaseRule; }

	const FuzzyWord* DispatchStringPointer(const char *text) {
		char param[32] = { }, value[32] = { };
		sscanf(text, "%s %s", param, value);
		auto *p = FindParameter(param);
		if(!p) return 0;
		auto *v = p->get(value);
		return v;
	}

	void PushResult(const FuzzyParameter *p, float f) { 
		m_results.insert(std::make_pair(p, f));
	}
	const ResultContainer& Results() const { return m_results; }

	void DumpValues() const {
		printf("Values:\n");
		for(auto &it: m_values) {
			printf("\t(%0.2f)%s\t%s\n", it.value, it.Word->Name.c_str(), it.Word->Owner->GetName().c_str());
		}
	}
	void DumpParams() const {
		printf("Parameters:\n");
		for(auto &it: m_params) {
			auto &fp = it.second;
			printf("%s:\n", fp.GetName().c_str());
			for(auto &jt: fp.AccessWords()) {
				printf("\t%s\n", jt.first.c_str());
			}
		}
	}

	void ClearValues() {
		m_values.clear();
		m_results.clear();
	}

	~FuzzyWorld();
	FuzzyWorld();
private:
	ParameterVector m_params;
	FuzzyWordValueList m_values;
	FuzzyRuleContainer *m_BaseRule;
	ResultContainer m_results;
};

struct FuzzyCondition : public FuzzyRule {
	const FuzzyWord *word;
	FuzzyCondition(FuzzyWorld *w, const FuzzyWord *ww): FuzzyRule(w), word(ww) { }
	virtual FuzzyTestResult Test() const {
		try {
			auto *v = m_world->ValueOf(word);
			if(!v) throw false;
			return FuzzyTestResult(v->value, word);
		}
		catch(...) { }
		return FuzzyTestResult(RuleTestResult::ValueNotExists);
	}
};

struct FuzzyOperator : public FuzzyRule {
	typedef std::vector<FuzzyRule*> RuleVector;
	virtual ~FuzzyOperator() {
		for(auto it: m_rules) delete it;
		m_rules.clear();
	}

	FuzzyOperator* add(FuzzyRule* r) {
		m_rules.push_back(r);
		return this;
	}
	FuzzyOperator* add(const char *text) {
		auto *w = m_world->DispatchStringPointer(text);
		if(w) add(new FuzzyCondition(m_world, w));
		return this;
	}
	FuzzyOperator* result(const char *text) {
		auto *w = m_world->DispatchStringPointer(text);
		m_result = w;
		return this;
	}
protected:
	static FuzzyMath *_Math;
	FuzzyOperator(FuzzyWorld *w): FuzzyRule(w), m_result(0) { }
	RuleVector m_rules;
	const FuzzyWord* m_result;
};
FuzzyMath* FuzzyOperator::_Math = new FuzzyClassicMath();

struct FuzzyAnd : public FuzzyOperator {
	FuzzyAnd(FuzzyWorld *w): FuzzyOperator(w){ }
	virtual FuzzyTestResult Test() const {
		FuzzyMath::MathInput r;
		for(auto *it: m_rules) {
			auto ret = it->Test();
			if(ret.result != RuleTestResult::Passed)
				return FuzzyTestResult(RuleTestResult::NotPassed);
			r.push_back(ret.value);
		}
		return FuzzyTestResult(_Math->And(r), m_result);
	}
};

struct FuzzyOr : public FuzzyOperator {
	FuzzyOr(FuzzyWorld *w): FuzzyOperator(w){ }
	virtual FuzzyTestResult Test() const {
		FuzzyMath::MathInput r;
		for(auto *it: m_rules) {
			auto ret = it->Test();
			if(ret.result != RuleTestResult::Passed)
				return FuzzyTestResult(RuleTestResult::NotPassed);
			r.push_back(ret.value);
		}
		return FuzzyTestResult(_Math->Or(r), m_result);
	}
};

struct FuzzyRuleContainer {
	typedef std::vector<FuzzyRule*> RuleVector;
	FuzzyRuleContainer(FuzzyWorld *w): m_world(w) { }
	virtual ~FuzzyRuleContainer() {
		for(auto it: m_rules) delete it;
		m_rules.clear();
	}

	FuzzyRuleContainer* add(FuzzyRule* r) {
		m_rules.push_back(r);
		return this;
	}

	void Process() {
		std::list<const FuzzyRule*> rules;
		for(auto &it: m_rules)
			rules.push_back(it);

		std::map<const FuzzyWord*, float> values;
		std::map<const FuzzyParameter*, std::vector<std::pair<const FuzzyWord*, float>>> params;

		while(!rules.empty()) {
			bool progress = false;
			for(auto it = rules.begin(), jt = rules.end(); it != jt; ++it) {
				auto r = (*it)->Test();
				if(r.result == RuleTestResult::Passed) {
					if(r.word) {
						if(values.find(r.word) == values.end()){
							auto & v = values[r.word];
							v = 0;
						}
						auto & v = values[r.word];
						if(v < r.value)
							v = r.value;
					}
					rules.erase(it);
					progress = true;
					break;
				}
			}

			if(progress) 
				continue;
			break;
		}

		for(auto &it: values) {
			params[it.first->Owner].push_back(std::make_pair(it.first, it.second));
			m_world->PushValue(it.first, it.second);
		}

		for(auto &it: params) {
			float val = it.first->ReverseProcess(it.second);

			m_world->PushResult(it.first, val);
		};
	}
protected:
	FuzzyWorld *m_world;
	RuleVector m_rules;
};

//-------------------------

FuzzyWorld::~FuzzyWorld() { delete m_BaseRule; }
FuzzyWorld::FuzzyWorld(): m_BaseRule(new FuzzyRuleContainer(this)){ }
