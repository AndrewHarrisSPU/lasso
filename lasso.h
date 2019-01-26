#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>

// anonymous namespace is accessible only in this translation unit
// so - only this header and any file that includes it
namespace {
	// formatted styles of text for terminal output
	namespace textStyle {
		std::string ok = "\e[1;32m";
		std::string fail = "\e[1;31m";
		std::string stealth = "\e[30m";
		std::string title = "\e[1m";
		std::string reset = "\e[0m";
	}

	// persistent test truth variable
	bool passed = true;
	int count = 0;

	void section( std::string label = "" ){
		if( label == "" )
			std::cout << "\n\n";
		else
			std::cout
				<< "\n\n "<< textStyle::title << "[ " << label << " ]"
				<< "\n" << textStyle::reset;

		count = 0;
	}
}

// EXPECT
// Swiss-army comparison function
// usage similar to assert() but doesn't kill process
// equality is stealthy, inequalilty is verbose
template< typename T, typename U >
void Expect( std::string file, int line, std::string name, T wanted, U got ){
	if( wanted != got ){
		std::cout
			<< textStyle::title
			<< "\n    ->  " << name << textStyle::reset
			<< textStyle::stealth
			<< "\n     -  " << "wanted  " << textStyle::reset << wanted
			<< textStyle::stealth
			<< "\n     -  " << "got     " << textStyle::reset << got
			<< textStyle::stealth
			<< "\n     @  " << file << ":" << line << textStyle::reset
			<< "\n";

		::passed = false;
		return;
	}
}

// Now define any call to expand with a macro (I know, bleh) to include file and line
#define Expect(...) Expect( __FILE__, __LINE__, __VA_ARGS__ )

// TEST CLASS
// Lasso<> can be used to test a void, no-arg function
// This is a very useful case.
// See examples for usage with more complicated type signatures.
template< typename T = void( * )(), typename ...Xs >
class lasso {
public:
	// CONSTRUCT
	lasso( std::string, T ); // name and pointer to lasso'd function

	// RUN
	// calls exec_, with args
	void Run( Xs... args );

	// TIMER
	void Start(); // starts timer
	void Stop(); // stops timer
	std::string Elapsed(); // (doesn't do anything. does report elapsed time.)

	// REPORT
	std::string TerminalString(); // Uses terminal color coding
	std::string RawString(); // Simpler string output

private:
	std::string name_;
	int index_; // ( magically assigned at construction )

	T exec_; // 'T' assumes a function pointer-ish type; exec_ is the function

	// start_, stop_, elapsed_
	std::chrono::time_point< std::chrono::high_resolution_clock > start_, stop_;
	std::chrono::duration<double> elapsed_;
};

// CONSTRUCT
template< typename T, typename ...Xs >
lasso< T, Xs... >::lasso( std::string lassoName, T exec ){
	// tests are indexed in order of creation
	::count++;
	index_ = ::count;

	name_ = lassoName;
	exec_ = exec;

	// Initializing to now() prevents garbage initializations from the future
	start_ = std::chrono::high_resolution_clock::now();
	stop_ = start_;
};

// RUN
template< typename T, typename ...Xs >
void lasso< T, Xs... >::Run( Xs... args ) {
	// reset ::passed, set start time, execute test, set end time, calculate duration
	::passed = true;

	Start();
	exec_( args... ); 
	Stop();

	return;
}

// TIMER
template< typename T, typename ...Xs >
void lasso< T, Xs... >::Start() {
	auto curr = std::chrono::high_resolution_clock::now();
	if( curr > start_ ) start_ = curr;
}

template< typename T, typename ...Xs >
void lasso< T, Xs... >::Stop() {

	auto curr = std::chrono::high_resolution_clock::now();
	if( stop_ <= start_ ) stop_ = curr;
	else if( curr < stop_ ) stop_ = curr;

	elapsed_ = stop_ - start_;
}

template< typename T, typename ...Xs >
std::string lasso< T, Xs... >::Elapsed() {
	std::stringstream ssout;

	ssout << std::setprecision( 10 ) << elapsed_.count();

	return ssout.str();
}

// REPORT
template< typename T, typename ...Xs >
std::string lasso< T, Xs... >::TerminalString() {
	std::stringstream ssout;

	ssout << "\n ";

	// label
	ssout
		<< textStyle::stealth << index_ << "/" << ::count << textStyle::reset
		<< " " << textStyle::title << name_ << textStyle::reset;
	
	std::string label = ssout.str();

	ssout.str( std::string() );

	ssout 
		<< std::setw( 48 ) << std::left << label;

	// result
	if( ::passed )
		ssout << textStyle::ok << "pass" << textStyle::reset;
	else
		ssout << textStyle::fail << "fail" << textStyle::reset;

	// timing
	ssout
		<< std::fixed << std::setprecision( 2 )
		<< " " << textStyle::stealth << elapsed_.count() << "s" << textStyle::reset
		<< "\n";

	return ssout.str();
}

template< typename T, typename ...Xs >
std::string lasso< T, Xs... >::RawString() {
	std::stringstream ssout;

	if( ::passed )
		ssout
			<< "\n" << name_
			<< std::setprecision( 6 )
			<< " " << elapsed_.count();
	else
		ssout
			<< "\n" << name_
			<< " " << "failed";

	return ssout.str();
}

// ERRORFOO
// easy-to-inherit error interface
class errorFoo {
public:
	void setErr( std::string error )
		{ if( error_ == "" ) error_ = error; }
	std::string getErr()
		{ return error_; }
	void flushErr()
		{ error_ = ""; }
	bool err()
		{ return ( error_ != "" ); }
private:
	std::string error_;
};