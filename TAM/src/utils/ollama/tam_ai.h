/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ai.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               AI handle declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 01.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_ai_h_
#define __tam_ai_h_

/* 'tam' namespace */
namespace tam
{
  /* 'ollama' handle class */
  class ollama
  {
  private:
    INT Port;                             // AI port
    std::wstring Host;                    // AI host
    PROCESS_INFORMATION OllamaProcess {}; // Ollama process
    BOOL OllamaStartedByUs = FALSE;       // Is started our Ollama process flag

    /* Build request for ai function.
     * ARGUMENTS:
     *   - Json file with room and furniture parametrs:
     *       const std::string &Input;
     * RETURNS:
     *   (std::string) Ready request for ai descriptions.
     */
    std::string BuildRequest( const std::string &Input ); 

    /* Send ready request to ai function.
     * ARGUMENTS:
     *   - Ready request for ai:
     *       const std::string &Request;
     * RETURNS:
     *   (std::string) ai response.
     */
    std::string SendRequest( const std::string &Request ); 

    /* Parse ai response function.
     * ARGUMENTS:
     *   - Ai response:
     *       const std::string &Response;
     * RETURNS:
     *   (std::string) parsed response.
     */
    std::string ParseResponse( const std::string &Response );

    /* Convert ai response to JSON function.
     * ARGUMENTS:
     *   - Ai response:
     *       const std::string &Response;
     * RETURNS:
     *   (std::string) converted JSON.
     */
    std::string ConvertResponse( const std::string &Response ); 

    /* Convert JSON to spesific request typr for ai function.
     * ARGUMENTS:
     *   - JSON input:
     *       const std::string &Input;
     * RETURNS:
     *   (std::string) parsed response.
     */
    std::string ConvertJson( const std::string &Input );

    /* Build request for ai function.
     * ARGUMENTS:
     *   - Request string:
     *       const std::string &Request;
     * RETURNS:
     *   (std::string) Ready request for ai descriptions.
     */
    std::string BuildTextRequest( const std::string &Request );

    /* Format text function.
     * ARGUMENTS:
     *   - Text to format:
     *       const std::string &Text;
     * RETURNS:
     *   (std::string) Formated text.
     */
    std::string FormatText( const std::string &Text );

    /* Start Ollama process function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) Is Ollama started.
     */
    BOOL StartOllamaProcess( VOID );

    /* Pull model in cmd function.
     * ARGUMENTS:
     *   - Model name:
     *       const std::string &Model;
     * RETURNS:
     *   (BOOL) If pulled succesfully.
     */
    BOOL PullModelCMD( const std::wstring &Model );

    /* Check if all ai parts are ready function.
     * ARGUMENTS:
     *   - Model name:
     *       const std::string &Model;
     * RETURNS:
     *   (BOOL) If ai is ready to use.
     */
    BOOL EnsureAll( const std::string &Model );

    /* Check if Ollama is running function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) If Ollama is running.
     */
    BOOL IsOllamaRunning( VOID );

    /* Check if Ollama has current model function.
     * ARGUMENTS:
     *   - Model name:
     *       const std::string &Model;
     * RETURNS:
     *   (BOOL) If Ollama has current model.
     */
    BOOL HasModel( const std::string &Model );

    /* Make a http GET request function.
     * ARGUMENTS:
     *   - Host to send a request:
     *       const std::wstring &Host;
     *   - API tags:
     *       const std::string &Path;
     * RETURNS:
     *   (std::string) raw JSON.
     */
    std::string HttpGet( const std::wstring &Host, const std::wstring &Path );

    /* Stop Ollama process function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (BOOL) If Ollama is stopped.
     */
    BOOL StopOllama( VOID );

  public:
    /* Default class constructor function.
     * ARGUMENTS:
     *   - Host:
     *       const std::string &NewHost;
     *   - Port:
     *       const INT NewPort;
     * RETURNS: None.
     */
    ollama( const std::wstring &NewHost = L"localhost", const INT NewPort = 11434) : Port(NewPort), Host(NewHost)
    {
      // Check API
      EnsureAll("deepseek-v3.1:671b-cloud");
      EnsureAll("llama3");
    } /* End of 'ollama' function */

    /* Default class destructor function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~ollama( VOID )
    {
      StopOllama();
    } /* End of '~ollama' function */

    /* Generate layout with ai function.
     * ARGUMENTS:
     *   - Json file with room and furniture parametrs:
     *       const std::string &Input;
     * RETURNS:
     *   (std::string) Json file with arranged furniture descriptions.
     */
    std::string GenerateLayout( const std::string &Input ); 

    /* Generate answer with ai function.
     * ARGUMENTS:
     *   - Input request:
     *       const std::string &Input;
     * RETURNS:
     *   (std::string) response string.
     */
    std::string GenerateAnswer( const std::string &Input ); 
  }; /* End of 'ollama' class */
} /* end of 'tam' namespace */

#endif /* __tam_ai_h_ */

/* END OF 'tam_ai.h' FILE */
