#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "noncopyable.h"
#include "referee.pb.h"
#include "savestate.pb.h"
#include "timing.h"
#include <cstdint>
#include <string>
#include <vector>
#include <glibmm/ustring.h>
#include <sigc++/connection.h>
#include <sigc++/signal.h>

class Configuration;
class GameInfo;
class Logger;
class Publisher;

class GameController : public NonCopyable {
	public:
		SaveState state;
		const Configuration &configuration;
		sigc::signal<void> signal_timeout_time_changed, signal_game_clock_changed, signal_yellow_card_time_changed, signal_other_changed;

		GameController(Logger &logger, const Configuration &configuration, const std::vector<Publisher *> &publishers, const std::string &resume_filename);
		~GameController();

		bool can_enter_stage(SSL_Referee::Stage stage) const;
		void enter_stage(SSL_Referee::Stage stage);
		SSL_Referee::Stage next_half_time() const;

		bool can_set_command(SSL_Referee::Command command) const;
		void set_command(SSL_Referee::Command command);

		void set_teamname(SaveState::Team team, const Glib::ustring &name);

		bool can_set_goalie() const;
		void set_goalie(SaveState::Team team, unsigned int goalie);

		bool can_switch_colours() const;
		void switch_colours();

		bool can_subtract_goal(SaveState::Team team) const;
		void subtract_goal(SaveState::Team team);

		void cancel_card_or_timeout();

		bool can_issue_card() const;
		void yellow_card(SaveState::Team team);
		void red_card(SaveState::Team team);

	private:
		Logger &logger;
		const std::vector<Publisher *> &publishers;
		sigc::connection tick_connection;
		MicrosecondCounter timer;
		uint64_t microseconds_since_last_state_save;

		bool tick();
		void advance_from_pre();
};

#endif

