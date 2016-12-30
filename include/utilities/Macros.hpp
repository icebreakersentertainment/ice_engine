#ifndef MACROS_H_
#define MACROS_H_

#define GET(Type, MemberName, FaceName) \
	Type get##FaceName() const { \
		return MemberName; \
	};

#define SET(Type, MemberName, FaceName) \
	void set##FaceName(Type value) { \
		MemberName = value; \
	};

#define SET_CONST_REF(Type, MemberName, FaceName) \
	void set##FaceName(const Type& value) { \
		MemberName = value; \
	};

#define GET_CONST_REF(Type, MemberName, FaceName) \
	const Type& get##FaceName() const { \
		return MemberName; \
	};

#define SET_REF(Type, MemberName, FaceName) \
	void set##FaceName(Type& value) { \
		MemberName = value; \
	};

#define GET_REF(Type, MemberName, FaceName) \
	Type& get##FaceName() { \
		return MemberName; \
	};

#define GETSET(Type, MemberName, FaceName) \
	Type get##FaceName() const { \
		return MemberName; \
	}; \
	\
	void set##FaceName(Type value) { \
		MemberName = value; \
	};

#define GETSET_CONST_REF(Type, MemberName, FaceName) \
	const Type& get##FaceName() const { \
		return MemberName; \
	}; \
	\
	void set##FaceName(const Type& value) { \
		MemberName = value; \
	};

#define GETSET_REF(Type, MemberName, FaceName) \
	Type& get##FaceName() const { \
		return MemberName; \
	}; \
	\
	void set##FaceName(Type& value) { \
		MemberName = value; \
	};

#define GETSET_INTERFACE(Type, FaceName) \
	virtual Type get##FaceName() const = 0; \
	\
	virtual void set##FaceName(Type value) = 0;

/*
#define GETSET(ClassName, Type, MemberName, FaceName) \
	Type ClassName::get##FaceName() const { \
		return MemberName; \
	}; \
	\
	void ClassName::set##FaceName(Type value) { \
		MemberName = value; \
	}
*/


// Debug Macros
#define FILE_AND_LINE_NUMBER std::string(__FILE__) + " (" + std::to_string(__LINE__) + ")"

#define OPENGL_CHECK_ERRORS(openGlDevice) \
{ \
	glr::glw::GlError err = openGlDevice->getGlError(); \
	if (err.type != GL_NONE) \
	{ \
		std::string msg = FILE_AND_LINE_NUMBER + ": Error occured during OpenGL operation: " + err.name; \
		LOG_ERROR( msg ); \
		throw glr::exception::GlException( msg ); \
	} \
}

#endif /* MACROS_H_ */
