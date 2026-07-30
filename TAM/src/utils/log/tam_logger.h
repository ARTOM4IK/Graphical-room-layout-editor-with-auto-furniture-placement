/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_logger.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               Logger declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 05.03.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_logger_h_
#define __tam_logger_h_

#include "tam_def.h"

#define LOG_DEF_BACK_COLOR 0x030818

/* 'tam' namespace */
namespace tam
{
  /* 'logger_base' handle class */
  class logger_base
  {
  private:
    // // Console initialization flag
    // inline static BOOL IsConsoleInit = FALSE;

    inline static DWORD ColorFore;          // Foreground color
    inline static DWORD ColorBack;          // Background color

  public:
    inline static BOOL IsColorStr = FALSE;  // Flag for correct output after changing color

    /* Class default constructor */
    logger_base( VOID )
    {
    } /* End of 'logger_base' function */

    /* Class destructor */
    ~logger_base( VOID )
    {
    } /* End of '~logger_base' function */

    /* Change output color function.
     * ARGUMENTS:
     *   - foreground and background colors (RGB in DWORD pack):
     *       DWORD ForeColor, BackColor;
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string SetColorStr( DWORD ForeColor, DWORD BackColor = ColorBack )
    {
      ColorFore = ForeColor;
      ColorBack = BackColor;
      IsColorStr = TRUE;
      return std::format("\x1b[38;2;{};{};{}m\x1b[48;2;{};{};{}m",
        COM_LONGBYTE2(ForeColor), COM_LONGBYTE1(ForeColor), COM_LONGBYTE0(ForeColor),
        COM_LONGBYTE2(BackColor), COM_LONGBYTE1(BackColor), COM_LONGBYTE0(BackColor));
    } /* End of 'SetColorStr' function */

    /* Change foreground output color function.
     * ARGUMENTS:
     *   - foreground color (RGB in DWORD pack):
     *       DWORD ForeColor;
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string SetForeColorStr( DWORD ForeColor )
    {
      return SetColorStr(ForeColor, ColorBack);
    } /* End of 'SetForeColor' function */

    /* Change background output color function.
     * ARGUMENTS:
     *   - background color (RGB in DWORD pack):
     *       DWORD BackColor;
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string SetBackColorStr( DWORD BackColor )
    {
      return SetColorStr(ColorFore, BackColor);
    } /* End of 'SetBackColor' function */

    /* Change output background color to default function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string SetDefBackStr( VOID )
    {
      return SetBackColorStr(LOG_DEF_BACK_COLOR);
    } /* End of 'SetDefBack' function */

    /* Change output color function.
     * ARGUMENTS:
     *   - foreground and background colors (RGB in DWORD pack):
     *       DWORD ForeColor, BackColor;
     * RETURNS: None.
     */
    static VOID SetColor( DWORD ForeColor, DWORD BackColor = LOG_DEF_BACK_COLOR )
    {
      std::cout << SetColorStr(ForeColor, BackColor);
    } /* End of 'SetColor' function */

    /* Change foreground output color function.
     * ARGUMENTS:
     *   - foreground color (RGB in DWORD pack):
     *       DWORD ForeColor;
     * RETURNS: None.
     */
    static VOID SetForeColor( DWORD ForeColor )
    {
      SetColor(ForeColor, ColorBack);
    } /* End of 'SetForeColor' function */

    /* Change background output color function.
     * ARGUMENTS:
     *   - background color (RGB in DWORD pack):
     *       DWORD BackColor;
     * RETURNS: None.
     */
    static VOID SetBackColor( DWORD BackColor )
    {
      SetColor(ColorFore, BackColor);
    } /* End of 'SetBackColor' function */

    /* Change output color to default function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    static VOID SetDef( VOID )
    {
      SetColor(0xFFFFFF, LOG_DEF_BACK_COLOR);
      IsColorStr = FALSE;
    } /* End of 'SetDef' function */

    /* Change output background color to default function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    static VOID SetDefBack( VOID )
    {
      SetBackColor(LOG_DEF_BACK_COLOR);
    } /* End of 'SetDefBack' function */

    /* Change output color to default function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) string with default color.
     */
    static std::string SetDefStr( VOID )
    {
      return SetColorStr(0xFFFFFF, LOG_DEF_BACK_COLOR);
    } /* End of 'SetDef' function */

    /* Change output color function.
     * ARGUMENTS:
     *   - foreground and background colors (RGB in DWORD pack):
     *       DWORD ForeColor, BackColor;
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string Color( DWORD ForeColor, DWORD BackColor = ColorBack )
    {
      ColorFore = ForeColor;
      ColorBack = BackColor;
      IsColorStr = TRUE;
      return std::format("\x1b[38;2;{};{};{}m\x1b[48;2;{};{};{}m",
        COM_LONGBYTE2(ForeColor), COM_LONGBYTE1(ForeColor), COM_LONGBYTE0(ForeColor),
        COM_LONGBYTE2(BackColor), COM_LONGBYTE1(BackColor), COM_LONGBYTE0(BackColor));
    } /* End of 'Color' function */

    /* Change foreground output color function.
     * ARGUMENTS:
     *   - foreground color (RGB in DWORD pack):
     *       DWORD ForeColor;
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string ForeColor( DWORD ForeColor )
    {
      return SetColorStr(ForeColor, ColorBack);
    } /* End of 'ForeColor' function */

    /* Change background output color function.
     * ARGUMENTS:
     *   - background color (RGB in DWORD pack):
     *       DWORD BackColor;
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string BackColor( DWORD BackColor )
    {
      return SetColorStr(ColorFore, BackColor);
    } /* End of 'BackColor' function */

    /* Change output background color to default function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) colored string.
     */
    static std::string DefBack( VOID )
    {
      return SetBackColorStr(LOG_DEF_BACK_COLOR);
    } /* End of 'DefBack' function */

    /* Change output color to default function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) string with default color.
     */
    static std::string DefColor( VOID )
    {
      return SetColorStr(0xFFFFFF, LOG_DEF_BACK_COLOR);
    } /* End of 'DefOut' function */

    /* Outputing time function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    static VOID Time( VOID )
    {
      std::cout << SetColorStr(0x00FF33, LOG_DEF_BACK_COLOR);
      std::time_t t_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::cout << std::put_time(std::localtime(&t_c), "[%d.%m.%Y %H:%M:%S]");
      SetDef();
    } /* End of 'Time' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       std::string Str;
     *   - color of text:
     *       DWORD ForeColor;
     *   - color of background:
     *       DWORD BackColor;
     * RETURNS: None.
     */
    static VOID Log( std::string Str, DWORD ForeColor, DWORD BackColor = LOG_DEF_BACK_COLOR )
    {
      std::cout << SetColorStr(ForeColor, BackColor);
      std::cout << Str;
      std::cout << SetColorStr(0xFFFFFF);
    } /* End of 'Log' function */

  }; /* End of 'logger_base' class */

  /* 'logger_rec' handle class */
  class logger_rec : public logger_base
  {
  public:
    BOOL IsEnabled = TRUE;  // Is output enabled

    /* Class default constructor */
    logger_rec( VOID ) : logger_base()
    {
    } /* End of 'logger_rec' function */

    /* Class constructor.
     * ARGUMENTS:
     *   - flag will be outputted or not:
     *       BOOL IsUse;
     */
    logger_rec( BOOL IsUse ) : logger_base()
    {
      IsEnabled = IsUse;
    } /* End of 'logger_rec' function */

    /* Class destructor */
    ~logger_rec( VOID )
    {
    } /* End of '~logger_rec' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       std::string Str;
     * RETURNS:
     *   (logger_rec &) reference to this logger_rec.
     */
    logger_rec & operator<<( const std::string &Str )
    {
      if (!IsEnabled)
        return *this;
      std::cout << Str;
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       CHAR *Chs;
     * RETURNS:
     *   (logger_rec &) reference to this logger_rec.
     */
    logger_rec & operator<<( const CHAR *Chs )
    {
      if (!IsEnabled)
        return *this;
      std::cout << Chs;
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

    /* Change foreground color of output function.
     * ARGUMENTS:
     *   - new foreground color:
     *       DWORD Color;
     * RETURNS:
     *   (logger_rec &) reference to this logger_rec.
     */
    logger_rec & operator>>( const DWORD Color )
    {
      std::cout << SetColorStr(Color);
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing float function.
     * ARGUMENTS:
     *   - value to print:
     *       FLT F;
     * RETURNS:
     *   (logger_rec &) reference to this logger_rec.
     */
    logger_rec & operator<<( const FLT F )
    {
      if (!IsEnabled)
        return *this;
      std::cout << F;
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing double function.
     * ARGUMENTS:
     *   - value to print:
     *       DBL D;
     * RETURNS:
     *   (logger_out &) reference to this logger_rec.
     */
    logger_rec & operator<<( const DBL D )
    {
      if (!IsEnabled)
        return *this;
      std::cout << D;
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing int function.
     * ARGUMENTS:
     *   - value to print:
     *       INT I;
     * RETURNS:
     *   (logger_rec &) reference to this logger_rec.
     */
    logger_rec & operator<<( const INT I )
    {
      if (!IsEnabled)
        return *this;
      std::cout << I;
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing bool function.
     * ARGUMENTS:
     *   - value to print:
     *       bool B;
     * RETURNS:
     *   (logger_out &) reference to this logger_rec.
     */
    logger_rec & operator<<( const bool B )
    {
      if (!IsEnabled)
        return *this;
      
      if (B)
        std::cout << "TRUE";
      else
        std::cout << "FALSE";
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing std::endl function.
     * ARGUMENTS:
     *   - function of print:
     *       std::ostream &((*func)( std::ostream & ));
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( std::ostream &((*func)( std::ostream & )) )
    {
      if (!IsEnabled)
        return *this;
      std::cout << std::endl;
      if (IsColorStr)
        IsColorStr = FALSE;
      return *this;
    } /* End of 'operator<<' function */

  }; /* End of 'logger_rec' class */

  /* 'logger_fatal' handle class */
  class logger_fatal : public logger_base
  {
  public:
    /* Class default constructor */
    logger_fatal( VOID ) : logger_base()
    {
    } /* End of 'logger_fatal' function */

    /* Class destructor */
    ~logger_fatal( VOID )
    {
    } /* End of '~logger_fatal' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       std::string Str;
     * RETURNS:
     *   (logger_fatal &) reference to this logger.
     */
    logger_fatal & operator<<( const std::string &Str )
    {
      std::cout << std::endl;
      Time();
      SetColor(0x000000, 0xDD0000);
      std::cout << "|FATAL ERROR|";
      SetColor(0xFF0000, 0x080803);
      std::cout << " > ";
      std::cout << Str << std::endl;
      SetDef();
      throw std::runtime_error(Str);
      return *this;
    } /* End of 'operator<<' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       CHAR *Chs;
     * RETURNS:
     *   (logger_fatal &) reference to this logger.
     */
    logger_fatal & operator<<( const CHAR *Chs )
    {
      std::cout << std::endl;
      Time();
      SetColor(0x000000, 0xDD0000);
      std::cout << "|FATAL ERROR|";
      SetColor(0xFF0000, 0x080803);
      std::cout << " > ";
      std::cout << Chs << std::endl;
      SetDef();
      throw std::runtime_error(Chs);
      return *this;
    } /* End of 'operator<<' function */

  }; /* End of 'logger_fatal' class */

  /* 'logger_output' handle class */
  class logger_output : public logger_base
  {
  private:
    BOOL IsNewLine = TRUE;                 // Is outputting starts from new line
    std::string Name;                      // Name of type of output(ex. "|INFO|", "|ERROR|", ...)
    std::string SubName = "";              // Subname/Subtype of output
    DWORD ForeColor;                       // Foreground color
    DWORD BackColor = LOG_DEF_BACK_COLOR;  // Background color

    BOOL IsEnabled = TRUE;                 // Is output enabled

    logger_rec LoggerRec;                  // logger_rec for outputting
  public:
    /* Class default constructor */
    logger_output( VOID ) : Name(""), ForeColor(0xFFFFFF), BackColor(LOG_DEF_BACK_COLOR), SubName("")
    {
    } /* End of 'logger_output' function */

    /* Class default constructor.
     * ARGUMENTS:
     *   - flag for starting output from new line:
     *       BOOL IsNLine;
     *   - name of output:
     *       std::string Str;
     *   - output colors:
     *       DWORD Fore, Back;
     */
    logger_output( BOOL IsNLine, std::string Str, DWORD Fore, DWORD Back = LOG_DEF_BACK_COLOR, BOOL IsOnlyDebug = FALSE ) : IsNewLine(IsNLine), Name(Str), ForeColor(Fore), BackColor(Back), SubName("")
    {
      if (IsOnlyDebug)
        IsEnabled = FALSE;
      #ifdef _DEBUG
      IsEnabled = TRUE;
      #endif // _DEBUG
      LoggerRec.IsEnabled = IsEnabled;
    } /* End of 'logger_output' function */

    /* Class default constructor.
     * ARGUMENTS:
     *   - flag for starting output from new line:
     *       BOOL IsNLine;
     *   - name of output:
     *       CHAR *Chs;
     *   - output colors:
     *       DWORD Fore, Back;
     */
    logger_output( BOOL IsNLine, CHAR *Chs, DWORD Fore, DWORD Back = LOG_DEF_BACK_COLOR, BOOL IsOnlyDebug = FALSE ) : IsNewLine(IsNLine), ForeColor(Fore), BackColor(Back), SubName("")
    {
      Name = Chs;

      if (IsOnlyDebug)
        IsEnabled = FALSE;
      #ifdef _DEBUG
      IsEnabled = TRUE;
      #endif // _DEBUG
      LoggerRec.IsEnabled = IsEnabled;
    } /* End of 'logger_output' function */

    /* Add subname type of output function.
     * ARGUMENTS:
     *   - new name:
     *       CHAR *Chs;
     * RETURNS:
     *   (logger_output &) reference to this logger_output.
     */
    logger_output & operator>>( const CHAR *Chs )
    {
      SubName = Chs;
      return *this;
    } /* End of 'operator>>' function */

    /* Change foreground color of output function.
     * ARGUMENTS:
     *   - new foreground color:
     *       DWORD Color;
     * RETURNS:
     *   (logger_rec &) reference to final logger_rec.
     */
    logger_rec & operator>>( DWORD Color )
    {
      return ((*this) << "" >> Color);
    } /* End of 'operator>>' function */
    
    /* Add subname type of output function.
     * ARGUMENTS:
     *   - new name:
     *       std::string Str;
     * RETURNS:
     *   (logger_output &) reference to this logger_output.
     */
    logger_output & operator>>( const std::string Str )
    {
      SubName = Str;
      return *this;
    } /* End of 'operator>>' function */

    /* Outputing name of type of output function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID OutputName( VOID )
    {
      if (IsNewLine && Name != "")
      {
        std::cout << std::endl;
        Time();
      }
      else if (IsNewLine)
        std::cout << std::endl;
      if (Name != "")
      {
        SetColor(ForeColor, BackColor);
        std::cout << "|" << Name;
        if (SubName != "")
          std::cout << " > " << SubName, SubName = "";
        std::cout << "|";
      }
      SetDef();
      if (IsNewLine && Name != "")
        std::cout << " > ";
    } /* End of 'OutputName' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       CHAR *Chs;
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( const CHAR *Chs )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      std::cout << Chs;
      return LoggerRec;
    } /* End of 'operator<<' function */

    /* Outputing string function.
     * ARGUMENTS:
     *   - string to print:
     *       std::string Str;
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( const std::string Str )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      std::cout << Str;
      return LoggerRec;
    } /* End of 'operator<<' function */

    /* Outputing int value function.
     * ARGUMENTS:
     *   - value to print:
     *       INT I;
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( const INT I )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      std::cout << I;
      return LoggerRec;
    } /* End of 'operator<<' function */

    /* Outputing double value function.
     * ARGUMENTS:
     *   - value to print:
     *       DBL D;
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( const DBL D )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      std::cout << D;
      return LoggerRec;
    } /* End of 'operator<<' function */

    /* Outputing float value function.
     * ARGUMENTS:
     *   - value to print:
     *       FLT F;
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( const FLT F )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      std::cout << F;
      return LoggerRec;
    } /* End of 'operator<<' function */

    /* Outputing bool value function.
     * ARGUMENTS:
     *   - value to print:
     *       bool B;
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( const bool B )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      if (B)
        std::cout << "TRUE";
      else
        std::cout << "FALSE";
      return LoggerRec;
    } /* End of 'operator<<' function */

    /* Outputing std::endl function.
     * ARGUMENTS:
     *   - function of print:
     *       std::ostream &((*func)( std::ostream & ));
     * RETURNS:
     *   (logger_rec &) reference to logger_rec.
     */
    logger_rec & operator<<( std::ostream &((*func)( std::ostream & )) )
    {
      if (!IsEnabled)
        return LoggerRec;
      OutputName();
      std::cout << std::endl;
      return LoggerRec;
    } /* End of 'operator<<' function */
  }; /* End of 'logger_output' class */

  /* 'logger_console' handle class */
  class logger_console
  {
    inline static BOOL IsConsoleInit = FALSE;  // Console initialization flag
  public:

    /* Class default constructor */
    logger_console( VOID )
    {
      if (IsConsoleInit)
        return;
      IsConsoleInit = TRUE;
      AllocConsole();
      SetConsoleTitle("<| TAM CONSOLE |>");
      std::freopen("CONOUT$", "w", stdout);
      system("@chcp 1251 > nul");
      HINSTANCE hInstance = GetModuleHandle(nullptr);
      if (HRSRC hrsrc = FindResource(hInstance, "CGSG.ASCII", "RC_DATA"); hrsrc != nullptr)
        if (UINT size = SizeofResource(hInstance, hrsrc); size != 0)
          if (HGLOBAL hm = LoadResource(hInstance, hrsrc); hm != NULL)
            std::cout << static_cast<CHAR *>(LockResource(hm)), UnlockResource(hm);

      auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_FONT_INFOEX Font;
      Font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
      GetCurrentConsoleFontEx(hConsole, FALSE, &Font);
      Font.FontWeight = 1000;
      Font.dwFontSize.Y = 18;
      SetCurrentConsoleFontEx(hConsole, FALSE, &Font);
      std::cout << logger_base::SetColorStr(0x00EE30, LOG_DEF_BACK_COLOR);
      std::time_t t_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
      std::cout << std::put_time(std::localtime(&t_c), "[%d.%m.%Y %H:%M:%S]") << "<CONSOLE CREATED>\n" << logger_base::SetDefStr();
    } /* End of 'logger_console' function */

    /* Class destructor */
    ~logger_console( VOID )
    {
      if (!IsConsoleInit)
        return;
      IsConsoleInit = FALSE;
      std::fclose(stdout);
      FreeConsole();
    } /* End of '~logger_console' function */
  }; /* End of 'logger_console' class */

  /* 'logger' handle class */
  class logger : public logger_base
  {
  private:
    inline static logger_console Console;                                                      // Logger console

  public:
    inline static const DWORD DefFore = 0xFFFFFF;                                              // Default ouput foreground color

    inline static logger_fatal fatal;                                                          // Output for fatal errors (with throw)
    inline static logger_output info = logger_output(TRUE, "INFO", 0xFFFFFF, 0x0060DD);        // Output for info
    inline static logger_output error = logger_output(TRUE, "ERROR", 0xFFFFFF, 0xEE3000);      // Output for errors
    inline static logger_output warning = logger_output(TRUE, "WARNING", 0x000000, 0xFFFE00);  // Output for warnings
    inline static logger_output sys = logger_output(TRUE, "SYSTEM", 0xFFFFFF, 0x008808);       // Output for system info
    inline static logger_output line = logger_output(FALSE, "", 0xFFFFFF, LOG_DEF_BACK_COLOR); // Output in same(previous) line
    inline static logger_output out = logger_output(TRUE, "", 0xFFFFFF, LOG_DEF_BACK_COLOR);   // Default output
    inline static logger_output test = logger_output(TRUE, "TEST", 0xFFFFFF, 0x8900DA);        // Output for tests

    inline static logger_output debug_info = logger_output(TRUE, "INFO (DEBUG)", 0xFFFFFF, 0x0060DD, TRUE);        // Output for info
    inline static logger_output debug_error = logger_output(TRUE, "ERROR (DEBUG)", 0xFFFFFF, 0xEE3000, TRUE);      // Output for errors
    inline static logger_output debug_warning = logger_output(TRUE, "WARNING (DEBUG)", 0x000000, 0xFFFE00, TRUE);  // Output for warnings
    inline static logger_output debug_sys = logger_output(TRUE, "SYSTEM (DEBUG)", 0xFFFFFF, 0x008808, TRUE);       // Output for system info
    inline static logger_output debug_line = logger_output(FALSE, "", 0xFFFFFF, LOG_DEF_BACK_COLOR, TRUE);         // Output in same(previous) line
    inline static logger_output debug_out = logger_output(TRUE, "", 0xFFFFFF, LOG_DEF_BACK_COLOR, TRUE);           // Default output
    inline static logger_output debug_test = logger_output(TRUE, "TEST (DEBUG)", 0xFFFFFF, 0x8900DA, TRUE);        // Output for tests

    /* Class default constructor */
    logger( VOID )
    {
    } /* End of 'logger' function */

    /* Class destructor */
    ~logger( VOID )
    {
    } /* End of '~logger' function */
  }; /* End of 'logger' class */

} /* end of 'tam' namespace */

#endif /* __tam_logger_h_ */

/* END OF 'tam_logger.h' FILE */
