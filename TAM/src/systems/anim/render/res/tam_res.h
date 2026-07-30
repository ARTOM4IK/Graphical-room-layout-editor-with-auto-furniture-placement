/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_res.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Resource declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 24.02.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_res_h_
#define __tam_res_h_

#include "tam_def.h"
#include "utils/log/tam_logger.h"

/* 'tam' namespace */
namespace tam
{
  /* Forward reference */
  class render;
 
  /* Base resource class (should be base class for all resources) */
  class resource
  {
    template<typename entry_type, typename index_type>
      friend class resource_manager;
  protected:
    // Pointer to render
    render *Rnd {};

  public:

    /* Class constructor.
     * ARGUMENTS:
     *   - pointer to render:
     *       render *Rnd;
     */
    resource( render *RndPtr ) : Rnd(RndPtr)
    {
    } /* End of 'resource' function */

    resource( resource & ) = delete;
    resource operator=( resource & ) = delete;

    resource( resource && ) noexcept = default;
    resource & operator=( resource && ) = delete;
  }; /* End of 'resource' class */
 
  /* Resource manager class */
  template<typename entry_type, typename index_type = INT>
    class resource_manager
    {
      // Check for correct entry resource class based on resource
      static_assert(std::is_base_of_v<resource, entry_type>, "Resource store class should be derived from 'resource'");
 
    protected:
      using resource_type = std::shared_ptr<entry_type>;

      // Reference to render
      render &RndRef;
 
      /* Resource entries stock */
      std::unordered_map<index_type, resource_type> Stock;
 
      /* Manager reference counter */
      INT TotalRefCnt = 0;
 
      /* Allocate resource in stock function.
       * ARGUMENTS: None.
       * RETURNS: None.
       */
      resource_type Allocate( index_type Id = {} )
      {
        if constexpr (std::is_convertible_v<index_type, INT>)
        {
          Id = TotalRefCnt++;
          resource_type ref = std::make_shared<entry_type>(entry_type(&RndRef));
          ref->Id = Id;
          Stock.emplace(Id, ref);
          return ref;
        }
        else
        {
          if (!Id.size())
            return nullptr;
          if (auto it = Stock.find(Id); it != Stock.end())
            return it->second;

          resource_type ref = std::make_shared<entry_type>(entry_type(&RndRef));
          Stock.emplace(Id, ref); 
          return ref;
        }
      } /* End of 'Allocate' function */
 
      /* Class constructor.
       * ARGUMENTS:
       *   - render instance reference:
       *       render &Rnd;
       */
      resource_manager( render &Rnd ) : RndRef(Rnd)
      {
      } /* End of 'resource_manager' function */
 
      /* Class destructor */
      ~resource_manager( VOID )
      {
        Clear();
      } /* End of '~resource_manager' function */
 
      /* Clear manager stock function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (resource_manager &) self reference.
       */
      resource_manager & Clear( VOID )
      {
        Freeze();
        for (auto it = Stock.begin(); it != Stock.end(); ++it)
          it->second->Free();
        Stock.clear();
        return *this;
      } /* End of 'Clear' function */
 
    public:
      /* Find resource at stock function.
       * ARGUMENTS:
       *   - resource name to find:
       *       const std::string &Name;
       * RETURNS:
       *   (type *) reference to found elememt.
       */
      resource_type Find( const index_type &Name )
      {
        if (auto it = Stock.find(Name); it != Stock.end())
          return it->second;
        return nullptr;
      } /* End of 'Find' function */
 
      /* Entry delete function.
       * ARGUMENTS:
       *   - entry interface pointer:
       *       entry_type *Entry;
       * RETURNS:
       *   (resource_manager &) self reference.
       */
      resource_manager & Delete( resource_type &Entry )
      {
        if (!Entry || !Stock.size())
          return *this;
        if (Entry.use_count() > 2)
        {
          Entry.reset();
          return *this;
        }
        if constexpr (std::is_convertible_v<index_type, INT>)
        {
          auto find_entry =
            Stock.find(Entry->Id);
          if (find_entry != Stock.end())
            Stock.erase(find_entry);
        }
        else
        {
          auto find_entry =
            Stock.find(Entry->Name);
          if (find_entry != Stock.end())
            Stock.erase(find_entry);
        }
        Entry.reset();
        return *this;
      } /* End of 'Delete' function */

      /* Freeze resource stock: remove the only instance entries function.
       * ARGUMENTS: None.
       * RETURNS:
       *   (resource_manager &) self reference.
       */
      resource_manager & Freeze( VOID )
      {
        for (auto it = Stock.begin(); it != Stock.end();)
          if (it->second.use_count() == 1)
          {
            auto save_it = it;
            ++it;
            Stock.erase(save_it);
          }
          else
            ++it;
        return *this;
      } /* End of 'Freeze' function */
    }; /* End of 'resource_manager' class */	
} /* end of 'tam' namespace */

#endif /* __tam_res_h_ */

/* END OF 'tam_res.h' FILE */
