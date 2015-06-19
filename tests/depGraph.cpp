/*******************************************************************************
   Copyright (C) 2015 Dario Oliveri
   See copyright notice in LICENSE.md
*******************************************************************************/
#include <priv/InfectorTypes.hpp>
#include <typeindex>
#include <priv/DependencyDAG.hpp>
#include <priv/ConcreteContainer.hpp>
#include <assert.h>

using namespace Infector;
using namespace Infector::priv;

struct A{

};

struct B: public virtual A{

};

struct kk{

};

struct JJ: public virtual kk{

};

struct C{

};

struct D{

};


template< typename T, typename... Contracts>
void bindSingleAs( ConcreteContainer * p){

    TypeInfoP         types[ sizeof...( Contracts)]
                        { &typeid( Contracts)... };

    UpcastSignature upcasts[ sizeof...( Contracts)]
                        { &upcast< T, Contracts>... };

    p->bindSingleAs( &typeid(T), types, upcasts, sizeof...( Contracts));
}


int depGraph( int argc, char ** argv){
	
	DependencyDAG dag;
	ConcreteContainer co;

	bindSingleAs<B,  A>(&co);
	bindSingleAs<JJ, kk>(&co);
	
	dag.setGuard( &typeid(B));
	dag.setGuard( &typeid(JJ));
	dag.dependOn( &typeid(JJ), &typeid(A), &co);
	
	try{
		bindSingleAs<B,  A>(&co);
		assert(false); //test failed
	}
	catch(RebindEx &ex){
		
	}
	catch(...){
		assert(false); //test failed
	}
	
	try{
		dag.dependOn( &typeid(B), &typeid(kk), &co);
		assert(false); //test failed
	}
	catch(CircularDependencyEx &ex){
		
	}
	catch(...){
		assert(false); //test failed
	}
	
	bindSingleAs<C,  C>(&co);
	bindSingleAs<D, D>(&co);
	
	dag.setGuard( &typeid(C));
	dag.setGuard( &typeid(D));
	dag.dependOn( &typeid(D), &typeid(C), &co);
	
	try{
		dag.dependOn( &typeid(C), &typeid(D), &co);
		assert(false); //test failed
	}
	catch(CircularDependencyEx &ex){
		
	}
	catch(...){
		assert(false); //test failed
	}
	
	return 0;
}