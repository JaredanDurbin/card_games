#ifndef SETONCE_H
#define SETONCE_H

// ----------------------------------------------------------------------------
// Class block.
// ----------------------------------------------------------------------------

/// @brief This class is for a variable that we would like to set once
///        (hence the name) and then it cannot be changed agan.
/// @tparam T This class takes any data type.
template<typename T>
class SetOnce
{
   private:
      // ----------------------------------------------------------------------
      // Private Variables block.
      // ----------------------------------------------------------------------

      // The actual variable itself that should not change.
      T    val;
      // The flag on whether it has been set of not.
      // If 'true' the variable has been set and should be changed.
      // if 'false' the variable has not been set and can be.
      bool flag = false;
      // ----------------------------------------------------------------------

   public:
      // ----------------------------------------------------------------------
      // Constructors block.
      // ----------------------------------------------------------------------

      /// @brief This is the standard constructor. The flag has not been
      ///        tripped because the variable has not been set yet.
      SetOnce():
         flag(false)
      {}

      /// @brief This is the initialized constructor which initializes 
      ///        the variable immediately.
      /// @param init The variable we would like to initialize.
      SetOnce(T init): 
         val(init),
         flag(true)
      {}
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Operator Overloads block.
      // ----------------------------------------------------------------------

      /// @brief This is an overload for the set operator.
      ///        This will set the variable if the flag has not been tripped.
      ///        If the flag has been tripped then this will fail.
      /// @param other The variable we would like to set this to instance to.
      /// @return Standard return for the set operator.
      ///         A pointer to this class.
      SetOnce<T>& operator=(const T &other)
      {
         if (!flag)
         {
            val = other;
            flag = true;
         }

         return *this;
      }

      /// @brief This is an oveload for the equals operator.
      /// @param obj The object we are trying to see if it is the same as ours.
      /// @return Returns whether the values are equal or not.
      bool operator==(const T &obj)
      {
         if (val == obj)
         {
            return true;
         }
         return false;
      }

      /// @brief This is an oveload for the equals operator for when we are
      ///        comparing this with another "SetOnce" variable.
      /// @param obj The object we are trying to see if it is the same as ours.
      /// @return Returns whether the values are equal or not.
      bool operator==(const SetOnce<T> &obj)
      {
         if (val == obj.Get())
         {
            return true;
         }
         return false;
      }
      // ----------------------------------------------------------------------

      // ----------------------------------------------------------------------
      // Public Get Function block.
      // ----------------------------------------------------------------------
      
      /// @brief Standard "Get" function for the variable this is set to.
      /// @return Returns the variable itself.
      const T& Get() const { return val; }
      // ----------------------------------------------------------------------
      
}; // SetOnce Class
// ----------------------------------------------------------------------------

#endif