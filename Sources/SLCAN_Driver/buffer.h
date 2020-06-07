/*
 *  Software for Industrial Communication, Motion Control, and Automation
 *
 *  Copyright (C) 2002-2020  Uwe Vogt, UV Software, Berlin (info@uv-software.com)
 *
 *  This module is part of the SourceMedley repository.
 *
 *  This module is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This module is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this module.  If not, see <http://www.gnu.org/licenses/>.
 */
/** @file        buffer.h
 *
 *  @brief       Buffer for intertask communication.
 *
 *  @remarks     A producer thread writes data into the buffer, if it is empty.
 *               A consumer thread waits until data has been written into the
 *               buffer and reads them, or returns when a time-out occurs.
 *
 *  @author      $Author: eris $
 *
 *  @version     $Rev: 690 $
 *
 *  @defgroup    buffer Waitable Buffer
 *  @{
 */
#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


/*  -----------  options  ------------------------------------------------
 */


/*  -----------  defines  ------------------------------------------------
 */


/*  -----------  types  --------------------------------------------------
 */

typedef void *buffer_t;                 /**< buffer (opaque data type) */


/*  -----------  variables  ----------------------------------------------
 */


/*  -----------  prototypes  ---------------------------------------------
 */
#ifdef __cplusplus
extern "C" {
#endif

/** @brief       creates an instance of a waitable buffer (constructor).
 *
 *  @param[in]   size  - size of the buffer (number of bytes)
 *
 *  @returns     pointer to a buffer instance if successful, or NULL on error.
 *
 *  @note        System variable 'errno' will be set in case of an error.
 *
 *  @retval      EINVAL   - invalid argument (size)
 *  @retval      ENOMEM   - out of memory (insufficient storage space)
 *  @retval      'errno'  - error code from called system functions:
 *                          'pthread_mutex_init', 'pthread_cond_init'
 */
extern buffer_t buffer_create(size_t size);


/** @brief       destroys the buffer instance (destructor).
 *
 *  @param[in]   buffer  - pointer to a buffer instance
 *
 *  @returns     0 if successful, or a negative value on error.
 *
 *  @note        System variable 'errno' will be set in case of an error.
 *
 *  @retval      EFAULT   - bad address (invalid buffer instance)
 *  @retval      'errno'  - error code from called system functions:
 *                          'pthread_mutex_destroy', 'pthread_cond_destroy'
 */
extern int buffer_destroy(buffer_t buffer);


/** @brief       empties the content of the buffer.
 *
 *  @param[in]   buffer  - pointer to a buffer instance
 *
 *  @returns     the number of bytes removed from the buffer if successful, or
 *               a negative value on error.
 *
 *  @note        System variable 'errno' will be set in case of an error.
 *
 *  @retval      EFAULT   - bad address (invalid buffer instance)
 */
extern int buffer_clear(buffer_t buffer);


/** @brief       copies n data bytes into the buffer, if empty.
 *
 *  @remarks     The function copies not more data bytes than the size of the
 *               buffer allows (see parameter 'size' of 'buffer_create').
 *               @see buffer_create
 *
 *  @param[in]   buffer  - pointer to a buffer instance
 *  @param[in]   data    - data to be copied into the buffer
 *  @param[in]   nbytes  - number of bytes to be copied into the buffer
 *
 *  @returns     the number of bytes copied into the buffer if successful, or
 *               a negative value on error.
 *
 *  @note        System variable 'errno' will be set in case of an error.
 *
 *  @retval      EFAULT  - bad address (invalid buffer instance)
 *  @retval      EINVAL  - invalid argument (data or nbytes)
 *  @retval      EBUSY   - device/resoure busy (not empty)
 */
extern int buffer_put(buffer_t buffer, const void *data, size_t nbytes);


/** @brief       copies the data bytes from the buffer, if any.
 *
 *  @remark      The buffer content is entirely emptied after this.
 *
 *  @param[in]   buffer   - pointer to a buffer instance
 *  @param[out]  data     - pointer to an array into which the data are copied
 *  @param[in]   maxbytes - maximum number of bytes to be copied from the buffer
 *  @param[in]   timeout  - time to wait for data available in the buffer:
 *                               0 means the function returns immediately,
 *                               65535 means blocking read, and any other
 *                               value means the time to wait im milliseconds
 *
 *  @returns     the number of bytes copied from the buffer if successful, or
 *               a negative value on error.
 *
 *  @note        System variable 'errno' will be set in case of an error.
 *
 *  @retval      EFAULT    - bad address (invalid buffer instance)
 *  @retval      EINVAL    - invalid argument (data or maxbytes)
 *  @retval      ENOMSG    - no data available (polling)
 *  @retval      ETIMEDOUT - timed out (blocking read)
 */
extern int buffer_get(buffer_t buffer, void *data, size_t maxbytes, uint16_t timeout);


/** @brief       signals waiting objects, if any.
 *
 *  @param[in]   buffer  - pointer to a buffer instance
 *
 *  @returns     0 if successful, or a negative value on error.
 */
extern int buffer_signal(buffer_t buffer);


#ifdef __cplusplus
}
#endif
#endif /* BUFFER_H_INCLUDED */

/*  ----------------------------------------------------------------------
 *  Uwe Vogt,  UV Software,  Chausseestrasse 33 A,  10115 Berlin,  Germany
 *  Tel.: +49-30-46799872,  Fax: +49-30-46799873,  Mobile: +49-170-3801903
 *  E-Mail: uwe.vogt@uv-software.de,  Homepage: http://www.uv-software.de/
 */
