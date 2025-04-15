workspace "MovieTicketBookingSystem"
   configurations { "Debug", "Release" }
   location "build"

project "MovieTicketBookingSystem"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "main.cpp", "src/**.cpp", "include/**.h", "assets/**" }
   includedirs { "include" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
