#pragma once

namespace cobb {
   constexpr double pi = 3.14159265358979323846;
   //
   inline float degrees_to_radians(float degrees) {
      if (!degrees)
         return 0.0;
      return degrees * pi / 180.0;
   }
   //
   inline float radians_to_degrees(float radians) {
      if (!radians)
         return 0.0;
      return radians * 180.0 / pi;
   }
};