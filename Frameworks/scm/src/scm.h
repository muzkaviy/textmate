#ifndef OAKSCM_H_SUWJ53QQ
#define OAKSCM_H_SUWJ53QQ

#include "scm_ng.h"
#include "status.h"
#include "snapshot.h"
#include <plist/date.h>
#include <oak/debug.h>
#include <oak/callbacks.h>
#include <oak/duration.h>

namespace scm
{
	struct driver_t;

	struct info_t;
	typedef std::shared_ptr<info_t> info_ptr;

	struct PUBLIC callback_t
	{
		virtual void status_changed (scm::info_t const& info, std::set<std::string> const& changedPaths) = 0;
		virtual ~callback_t () { }
	};

	struct watcher_t;

	struct PUBLIC info_t
	{
		info_t (std::string const& wcPath, driver_t const* driver);

		std::string scm_name () const;
		std::string path () const;
		std::string branch () const;
		bool tracks_directories () const;
		status::type status (std::string const& path);
		std::map<std::string, std::string> variables () const;
		status_map_t files_with_status (int mask);

		void add_callback (callback_t* cb);
		void remove_callback (callback_t* cb);

	private:
		void setup ();
		bool _did_setup = false;

		std::string _wc_path;
		driver_t const* _driver;
		status_map_t _file_status;
		std::map<std::string, std::string> _variables;
		oak::date_t _updated;
		fs::snapshot_t _snapshot;

		std::shared_ptr<scm::watcher_t> _watcher;
		void callback (std::set<std::string> const& pathsChangedOnDisk);
		oak::callbacks_t<callback_t> _callbacks;

		static void update_status (std::string const& path, fs::snapshot_t const& snapshot, scm::status_map_t const& status, std::map<std::string, std::string> const& variables);
	};

	PUBLIC info_ptr info (std::string const& dir);
	PUBLIC status_map_t tracked_files (std::string const& dir, int mask);

} /* scm */

#endif /* end of include guard: OAKSCM_H_SUWJ53QQ */
