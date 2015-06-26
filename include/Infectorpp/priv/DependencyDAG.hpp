/*******************************************************************************
   Copyright (C) 2015 Dario Oliveri
   See copyright notice in LICENSE.md
*******************************************************************************/
#pragma once
#include <list>
#include <tuple>
#include <unordered_map>
#include "InfectorTypes.hpp"


namespace Infector {
namespace priv {
class ConcreteContainer;
	
/** Dependency Direct Acyclic Graph.
	This collect dependencies between concrete types only ( A bit pointless
	tracing dependencies between interfaces since interfaces has no ctor).
	A Dependency DAG lives in one context only (Context inheritance is resolved
	by context only by creating a SymbolTable)*/
class DependencyDAG{
	
	using EdgeMap = std::unordered_map< std::type_index, 
										std::list< TypeInfoP> >;

	EdgeMap 	dependencies;
	EdgeMap		dependants;
	
	TypeInfoP 		guard = nullptr;
	DependencyDAG*	parent = nullptr;

public:
	
	void setGuard( TypeInfoP g);
	
								//type resolution is container responsibility
	void dependOn( TypeInfoP wired, TypeInfoP abstractDep, 
					ConcreteContainer * container);
	
	void remove( TypeInfoP concrete);
	
	DependencyDAG( DependencyDAG * parent);
	~DependencyDAG() = default;
	
//private:

	void removeDependant( TypeInfoP wired, TypeInfoP abstractDep);

	void addDependency( TypeInfoP wired, TypeInfoP abstractDep);
	
	void addDependant( TypeInfoP wired, TypeInfoP abstractDep);

	void checkGuardBreaking( 	TypeInfoP currentNode,
								ConcreteContainer * container,
								int HARD_RECURSION_LIMIT);
								
	std::list<TypeInfoP> getDependencies( TypeInfoP); 
	//
	//when you realize your Typo was "TypO" instead of "TypE" u.u ...
};
	

} // namespace priv
} // namespace Infector