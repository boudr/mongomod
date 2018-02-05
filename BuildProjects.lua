solution "mongomod"

	language "C++"
	location ( "build/" .. os.get() .. "-" .. _ACTION )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "EnableSSE", "NoImportLib"}
	targetdir ( "lib/" .. os.get() .. "/" )
	includedirs {	"/usr/local/include/mongocxx/v_noabi",
                    "/usr/local/include/bsoncxx/v_noabi",
                    "/usr/local/include/libmongoc-1.0",
                    "/usr/local/include/libbson-1.0",
					"gmodlua/include/",
					"mongomod/src/"
				 }

	if os.get() == "macosx" or os.get() == "linux" then

		buildoptions{ "-std=c++11 -fPIC" }
		linkoptions{ "-fPIC -static-libstdc++" }

	end
	
	configurations
	{ 
		"Release"
	}
	platforms{
		"x32"
	}
	
	configuration "Release"
		defines { "NDEBUG" }
		flags{ "Optimize", "FloatFast" }

		if os.get() == "windows" then

			defines{ "WIN32" }

		elseif os.get() == "linux" then

			defines{ "LINUX" }

		elseif os.get() == "macos" then
			defines{ "MACOS" }
		end
	project "gmsv_mongomod"
		defines{ "GMMODULE" }
		files{ "mongomod/src/**.*" }
		kind "SharedLib"
		libdirs { "/usr/local/lib" }
		local platform
		if os.get() == "windows" then
			error "Unsupported platform."
		elseif os.get() == "macosx" then
			platform = "osx"
		elseif os.get() == "linux" then
			platform = "linux"
		else
			error "Unsupported platform."
		end
		targetname( "gmsv_mongomod_" .. platform)
		targetprefix ("")
		targetextension(".dll")
		targetdir("build/lib/" .. os.get())
		
		if os.get() == "linux" then
			links { "mongocxx", "bsoncxx" }
		end
		
