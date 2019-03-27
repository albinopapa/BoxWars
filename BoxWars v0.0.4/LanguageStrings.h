#pragma once

enum class Language
{
	English, French, German, Italian, Spanish, Portuguese, Count
};


namespace MenuString
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "English"; 
			case Language::French:     return "Francais";
			case Language::German:     return "Deutsch"; 
			case Language::Italian:    return "Italiano";
			case Language::Spanish:    return "Espanol"; 
			case Language::Portuguese: return "Portugues"; 
			default: return nullptr;
		}
	}
}

namespace NewGame
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "New Game"; 
			case Language::French:     return "Nouveau Jeu"; 
			case Language::German:     return "Neues Spiel"; 
			case Language::Italian:    return "Nuovo Gioco"; 
			case Language::Spanish:    return "Nuevo Juego"; 
			case Language::Portuguese: return "Novo Jogo"; 
			default: return nullptr;
		}
	}
}

namespace LoadGame
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Load Game";
			case Language::French:     return "Charger une partie";
			case Language::German:     return "Spiel Laden"; 
			case Language::Italian:    return "Carica Gioco";
			case Language::Spanish:    return "Carga De Juegos";
			case Language::Portuguese: return "Carregar Jogo"; 
			default: return nullptr;
		}
	}
}

namespace ScoreBoard
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Scoreboard";
			case Language::French:     return "Tableau de bord"; 
			case Language::German:     return "Anzeigetafel"; 
			case Language::Italian:    return "Tabellone Segnapunti"; 
			case Language::Spanish:    return "Marcador"; 
			case Language::Portuguese: return "Scoreboard"; 
			default: return nullptr;
		}
	}
}
namespace Options
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Options"; 
			case Language::French:     return "Options";
			case Language::German:     return "Optionen"; 
			case Language::Italian:    return "Opzioni";
			case Language::Spanish:    return "Opciones"; 
			case Language::Portuguese: return "Opcoes";
			default: return nullptr;
		}
	}
}

namespace Credits
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Credits";
			case Language::French:     return "Credits";
			case Language::German:     return "Credits";
			case Language::Italian:    return "Crediti";
			case Language::Spanish:    return "Creditos";
			case Language::Portuguese: return "Creditos";
			default: return nullptr;
		}
	}
}

namespace Help
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Help";
			case Language::French:     return "Aide";
			case Language::German:     return "Hilfe";
			case Language::Italian:    return "Aiuto";
			case Language::Spanish:    return "Ayuda";
			case Language::Portuguese: return "Ajuda";
			default: return nullptr;
		}
	}
}

namespace Exit
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Exit";
			case Language::French:     return "Sortie";
			case Language::German:     return "Verlassen";
			case Language::Italian:    return "Uscita";
			case Language::Spanish:    return "Salide";
			case Language::Portuguese: return "Sair";
			default: return nullptr;
		}
	}
}

namespace CurrentScore
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Current Score";
			case Language::French:     return "Score Actuel"; 
			case Language::German:     return "Aktuelle Punktzahl"; 
			case Language::Italian:    return "Punteggio Attuale"; 
			case Language::Spanish:    return "Calificacion Actual";
			case Language::Portuguese: return "Pontuacao Atual"; 
			default: return nullptr;
		}
	}
}

namespace CurrentHealth
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Current Health";
			case Language::French:     return "De sante Actuel";
			case Language::German:     return "Aktuelle Gesundheits";
			case Language::Italian:    return "Salute Corrente";
			case Language::Spanish:    return "Salud Actual";
			case Language::Portuguese: return "De Saude Atual";
			default: return nullptr;
		}
	}
}

namespace CurrentMode
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:    return "Current Mode";
			case Language::French:     return "En Mode De Courant";
			case Language::German:     return "Aktuellen Modus";
			case Language::Italian:    return "Modalita' Corrente";
			case Language::Spanish:    return "Modo Actual";
			case Language::Portuguese: return "Modo Atual";
			default: return nullptr;
		}
	}
}

namespace TrivialMode
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		[[ fallthrough ]];
			case Language::French:		[[ fallthrough ]];   
			case Language::German:		[[ fallthrough ]];   
			case Language::Spanish:		[[ fallthrough ]];   
			case Language::Portuguese:  return "Trivial";
			case Language::Italian:    return "Banale";
			default: return nullptr;
		}
	}
}

namespace ChallengingMode
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Challenging";
			case Language::French:		return "Stimulant";
			case Language::German:		return "Anspruchsvoll";
			case Language::Italian:		return "Impegnativo";
			case Language::Spanish:		return "Desafiante";
			case Language::Portuguese:  return "Desafiante"; 
			default: return nullptr;
		}
	}
}

namespace HardlyDoableMode
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Hardly Doable";
			case Language::French:		return "Guere Faisable";
			case Language::German:		return "Kaum Machbar";
			case Language::Italian:		return "Difficilmente Fattibile";
			case Language::Spanish:		return "Dificilmente Realizable";
			case Language::Portuguese:  return "Dificilmente Factivel";
			default: return nullptr;
		}
	}
}

namespace ImpossibleMode
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Impossible";
			case Language::French:		return "Impossible";
			case Language::German:		return "Unmoglich";
			case Language::Italian:		return "Impossibile"; 
			case Language::Spanish:		return "Imposible";
			case Language::Portuguese:  return "Dificilmente Factivel"; 
			default: return nullptr;
		}
	}
}

namespace GoBackToMenu
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Go Back To Menu";
			case Language::French:		return "Revenir Au Menu";
			case Language::German:		return "Zuruck Zum Menu";
			case Language::Italian:		return "Tornare al Menu";
			case Language::Spanish:		return "Volver al menu";
			case Language::Portuguese:  return "Voltar Ao Menu";
			default: return nullptr;
		}
	}
}

namespace Credits_Tribute
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		[[ fallthrough ]];
			case Language::French:		[[ fallthrough ]];
			case Language::German:		[[ fallthrough ]];
			case Language::Italian:		[[ fallthrough ]];
			case Language::Spanish:		[[ fallthrough ]];
			case Language::Portuguese:  return "[BoxWar] v. 0.0.3 By Clodi";
			default: return nullptr;
		}
	}
}

namespace Credits_Contact
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "All Comments, Insults And Suggestions To: CXC129@bham.ac.uk";
			case Language::French:		return "Tous Les Commentaires, Les Insultes Et Suggestions A': CXC129@bham.ac.uk";
			case Language::German:		return "Alle Kommentare, Beleidigungen Und Anregungen An: CXC129@bham.ac.uk";
			case Language::Italian:		return "Ogni commento, Insulto O Suggerimento A: CXC129@bham.ac.uk";
			case Language::Spanish:		return "Todos Los Comentarios, Insultos Y Sugerencias A: CXC129@bham.ac.uk";
			case Language::Portuguese:  return "Todos Os Comentarios, Insultos E Sugestoes Para: CXC129@bham.ac.uk";
			default: return nullptr;
		}
	}
}

namespace PressEnter
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Press Enter";
			case Language::French:		return "Appuyez Sur Entree";
			case Language::German:		return "Drucken Sie Die Eingabetaste";
			case Language::Italian:		return "Premere Invio";
			case Language::Spanish:		return "Pulse Intro";
			case Language::Portuguese:  return "Pressione Enter";
			default: return nullptr;
		}
	}
}

namespace Legend_You
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "This Is You";
			case Language::French:		return "C'Est Vous";
			case Language::German:		return "Das Bist Du";
			case Language::Italian:		return "Questo Sei Tu";
			case Language::Spanish:		return "Esto Es";
			case Language::Portuguese:  return "Este E Voce";
			default: return nullptr;
		}
	}
}

namespace Legend_Target
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Fetch Your Target! It Gives You More Health";
			case Language::French:		return "Ceci Est Votre Cible, Il Donne Plus De Sante'";
			case Language::German:		return "Dies Ist Ihr Ziel, Es Gibt Zusatzliche Gesundheit";
			case Language::Italian:		return "Questo E' Il Vostro Obiettivo: Aggiunge Salute";
			case Language::Spanish:		return "Este Es Su Objetivo, Le Da Mas Salud";
			case Language::Portuguese:  return "Este E' O Seu Alvo, Ele Da' Mais Saude";
			default: return nullptr;
		}
	}
}

namespace Legend_Arena
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Danger Zone: Where Enemies Are";
			case Language::French:		return "Secteur A Risque: Ou' Les Ennemis Sont";
			case Language::German:		return "Gefahrenbereich: Wo Feinde sind";
			case Language::Italian:		return "Zona Di Pericolo: Dove I Nemici Sono";
			case Language::Spanish:		return "Zona De Peligro: Donde Los Enemigos Son";
			case Language::Portuguese:  return "Zona De Perigo: Onde Os Inimigos Sao";
			default: return nullptr;
		}
	}
}

namespace HelpMenu_Movement
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Use Arrow Keys To Move, Push Enter To Select";
			case Language::French:		return "Utilisez Les Flechees Pour Se Deplacer, ENTER Pour Selectionner";
			case Language::German:		return "Mit Den Pfeiltasten Zu Bewegen, Geben Sie Zu Wahlen";
			case Language::Italian:		return "Utilizzare I Tasti Freccia Per Spostarsi, INVIO Per Selezionare";
			case Language::Spanish:		return "Utilice Las Teclas De Flecha Para Mover, ENTER Para Seleccionar";
			case Language::Portuguese:  return "Use As Setas Para Mover, Pressione ENTER Para Selecionar";
			default: return nullptr;
		}
	}
}

namespace GameOver
{
	constexpr const char* GetString( Language _lang )noexcept
	{
		switch( _lang )
		{
			case Language::English:		return "Game Over";
			case Language::French:		return "Jeu Fini";
			case Language::German:		return "Game Over";
			case Language::Italian:		return "Fine Del Gioco";
			case Language::Spanish:		return "Game Over";
			case Language::Portuguese:  return "Ao Longo Do Jogo";
			default: return nullptr;
		}
	}
}