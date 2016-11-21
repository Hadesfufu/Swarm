#ifndef E_COMMAND_H
#define E_COMMAND_H

#include <exception>
#include <functional>

////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////

//	c'set moi ou elle est pas hyper utile cette classe?

template<class Receiver>
class E_Base : public std::exception{
public:
	E_Base(){};
	virtual std::function<void(Receiver*)> getFunction() = 0;
	virtual void setIssuer(void*) = 0;
	virtual const char* what() const{
		return typeid(Receiver).name();
	};
};

////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////

template<class Issuer, class Receiver>
class E_Command : public E_Base<Receiver>{
public:
	////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////

	E_Command(void(Receiver::* f)(Issuer*), Issuer* sender = nullptr) throw() :
		m_FunctionI(std::mem_fn(f)),
		m_FunctionN(nullptr)
	{
		setIssuer(sender);
	};

	E_Command(void(Receiver::* f)(void)) throw() :
		m_FunctionN(std::mem_fn(f)),
		m_FunctionI(nullptr)
	{
		if (!m_FunctionN)
			Log::error() << "Function N is not set, set the issuer";
	};
	

	std::function<void(Receiver*)> getFunction(){
		if (!m_FunctionN)
			Log::error() << "Function N is not set, set the issuer";
		
		return m_FunctionN;
	};

	void setIssuer(void* ptr){
		if (ptr){
			Issuer* issuer = static_cast<Issuer*>(ptr);
			if (!m_FunctionN && m_FunctionI)
				m_FunctionN = std::bind(m_FunctionI, std::placeholders::_1, issuer);
		}
	};

private:
	std::function<void(Receiver*, Issuer*)> m_FunctionI;
	std::function<void(Receiver*)>			m_FunctionN;
};

#endif 