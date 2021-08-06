#include "cli/psh_cli.h"
#include "fs/psh_fs.h"

static psh_cli *ctx;

int psh_init_cli(psh_cli *cli)
{
    if (cli && cli->input.buffer && cli->params.buffer)
    {
        ctx = cli;
        ctx->input._cnt = 0;
        ctx->params._cnt = 0;

        psh_print(ctx->ps1);

        return PSH_OK;
    }
    return PSH_ERROR_MEM;
}

int psh_print(const char *text)
{
    psh_file *f = psh_find_file(PSH_STD_OUT_NAME);
    if (f)
    {
        while (*text)
        {
            int result = psh_write(f, (char *)text++, 1);
            if (result < PSH_OK)
            {
                return result;
            }
        }
        return PSH_OK;
    }
    return PSH_FILE_NOT_OPEN;
}

int psh_process(void)
{

    int ret_val = 0;
    char c;
    psh_file *in = psh_open(PSH_STD_IN_NAME);
    int result = psh_read(in, &c, 1);

    if (result == 1)
    {
        if (c == ctx->new_line)
        {
            ctx->input.buffer[ctx->input._cnt] = '\0';
            char *current_pos = ctx->input.buffer;
            char *end_of_line = current_pos + ctx->input._cnt;
            ctx->params._cnt = 0;
            while ((ctx->params._cnt < ctx->params.size) && (current_pos < end_of_line))
            {
                while (*current_pos && (*current_pos == ctx->delimiter))
                    current_pos++;

                if (current_pos > end_of_line)
                    break;

                ctx->params.buffer[ctx->params._cnt++] = current_pos;

                while (*current_pos && (*current_pos != ctx->delimiter))
                    current_pos++;

                *(current_pos++) = '\0';
            }
            psh_file *file = psh_find_file(ctx->params.buffer[0]);
            if (file && file->driver.execute)
            {
                psh_file_frame *frame = psh_get_file_frame(file);
                ret_val = file->driver.execute(frame, ctx->params._cnt, ctx->params.buffer);
            }
            else
            {
                ret_val = -1;
            }

            ctx->input._cnt = 0;
            psh_print(ctx->ps1);
        }
        else
        {
            ctx->input.buffer[ctx->input._cnt++] = c;
        }
    }
    return ret_val;
}
