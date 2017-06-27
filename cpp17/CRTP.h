#ifndef CRTP_H
#define CRTP_H

#include <type_traits>

#ifndef HAS_FUNC
#define HAS_FUNC(_func) \
template<typename T, typename Result = void, typename ...Args> \
struct Has_##_func { \
	template <typename U, Result(U::*)(Args...)> struct Check; \
    template <typename U> static std::true_type test(Check<U, &U::_func>*); \
    template <typename U> static std::false_type test(...); \
	static constexpr bool value = std::is_same<decltype(test<T>(0)), std::true_type>::value; }; //  enum {value = std::is_same<decltype(func<T>(0)), std::true_type>::value};
#endif

#ifndef EXEC_IF_EXISTS_IMPL
#define EXEC_IF_EXISTS_IMPL(_name) \
template <typename T, typename Result = void, typename ...Args> \
typename std::enable_if<Has_##_name<T, Result, Args&&...>::value, Result>::type exec_if_exists_##_name(T& t, Result _default = {}, Args&& ...args) { return t._name(std::forward<Args>(args)...); } \
template <typename T, typename Result = void, typename ...Args> \
typename std::enable_if<!Has_##_name<T, Result, Args&&...>::value, Result>::type exec_if_exists_##_name(T& t, Result _default = {}, Args&& ...args) { return _default; }
#endif

#ifndef EXEC_IF_EXISTS
#define EXEC_IF_EXISTS(_name) HAS_FUNC(_name) EXEC_IF_EXISTS_IMPL(_name)
#endif

struct Camera
{
	// mvp etc
};

struct RenderObject
{
	//... pos, orientation, bones
};

class RenderPass
{
public:
	virtual bool OnRenderObject(const RenderObject& _Obj) { return false; }
	virtual bool OnRenderBatch(const std::vector<RenderObject>& _Objects, const Camera& _Cam) { return true; }

	void RenderBatch(const std::vector<RenderObject>& _Objects, const Camera& _Cam)
	{
		// set camera constants etc...
		if (OnRenderBatch(_Objects, _Cam) == false)
			return;

		for (const RenderObject& Obj : _Objects)
		{
			if (OnRenderObject(Obj) == false)
				continue; // skip object

			Render(Obj);
		}
	}
private:

	void Render(const RenderObject& _Object)
	{
		// ... do the real rendering here
	}
};

template <typename TPass>
class CRTPRenderPass
{
	EXEC_IF_EXISTS(OnRenderObject)
	EXEC_IF_EXISTS(OnRenderBatch)

public:
	bool RenderBatch(const std::vector<RenderObject>& _Objects, const Camera& _Cam)
	{
		if (exec_if_exists_OnRenderBatch(This(), false, _Objects, _Cam) == false)
			return false;
		
		for (const RenderObject& Obj : _Objects)
		{
			if (exec_if_exists_OnRenderObject(This(), true, Obj) == false)
				continue; // skip

			Render(Obj);
		}

		return true;
	}
private:
	TPass& This() { return static_cast<TPass&>(*this); }
	const TPass& This() const { return static_cast<const TPass&>(*this); }

	void Render(const RenderObject& _Object)
	{
		// ... do the real rendering here
	}
};

class MyRenderPass : public CRTPRenderPass<MyRenderPass>
{
public:
	inline bool OnRenderObject(const RenderObject& _Obj) { return true; }
	//inline bool OnRenderBatch(const std::vector<RenderObject>& _Objects, const Camera& _Cam) { return true; }
private:

};

#endif // !CRTP_H
