/* This file is is generated by a shell script.  DO NOT EDIT! */

/* 32 bit ELF emulation code for elf_i386_kot
   Copyright (C) 1991-2020 Free Software Foundation, Inc.
   Written by Steve Chamberlain <sac@cygnus.com>
   ELF support by Ian Lance Taylor <ian@cygnus.com>

   This file is part of the GNU Binutils.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#define TARGET_IS_elf_i386_kot

#include "sysdep.h"
#include "bfd.h"
#include "libiberty.h"
#include "getopt.h"
#include "bfdlink.h"
#include "ctf-api.h"
#include "ld.h"
#include "ldmain.h"
#include "ldmisc.h"
#include "ldexp.h"
#include "ldlang.h"
#include "ldfile.h"
#include "ldemul.h"
#include <ldgram.h>
#include "elf-bfd.h"
#include "ldelf.h"
#include "ldelfgen.h"

/* Declare functions used by various EXTRA_EM_FILEs.  */
static void gldelf_i386_kot_before_parse (void);
static void gldelf_i386_kot_after_open (void);
static void gldelf_i386_kot_before_allocation (void);
static void gldelf_i386_kot_after_allocation (void);


#include "ldlex.h"
#include "elf-linker-x86.h"

static struct elf_linker_x86_params params;

/* This is a convenient point to tell BFD about target specific flags.
   After the output has been created, but before inputs are read.  */

static void
elf_x86_create_output_section_statements (void)
{
  _bfd_elf_linker_x86_set_options (&link_info, &params);
}


static void
elf_x86_before_parse (void)
{
  params.call_nop_byte = 0x67;

  gldelf_i386_kot_before_parse ();
}


static void
gldelf_i386_kot_before_parse (void)
{
  ldfile_set_output_arch ("i386", bfd_arch_i386);
  input_flags.dynamic = TRUE;
  config.has_shared = TRUE;
  config.separate_code = FALSE;
  link_info.check_relocs_after_open_input = TRUE;
  link_info.relro = DEFAULT_LD_Z_RELRO;
  link_info.separate_code = DEFAULT_LD_Z_SEPARATE_CODE;
}


/* These variables are used to implement target options */

static char *audit; /* colon (typically) separated list of libs */
static char *depaudit; /* colon (typically) separated list of libs */


/* This is called after all the input files have been opened.  */

static void
gldelf_i386_kot_after_open (void)
{
  ldelf_after_open (FALSE, FALSE,
		    FALSE, FALSE, 32, "/home/sebastien/Bureau/Kot/Toolchain/Local");
}


/* This is called after the sections have been attached to output
   sections, but before any sizes or addresses have been set.  */

static void
gldelf_i386_kot_before_allocation (void)
{
  ldelf_before_allocation (audit, depaudit, NULL);
}


static void
gldelf_i386_kot_after_allocation (void)
{
  int need_layout = bfd_elf_discard_info (link_info.output_bfd, &link_info);

  if (need_layout < 0)
    einfo (_("%X%P: .eh_frame/.stab edit: %E\n"));
  else
    ldelf_map_segments (need_layout);
}

static char *
gldelf_i386_kot_get_script (int *isfile)
{
  *isfile = 1;

  if (bfd_link_relocatable (&link_info) && config.build_constructors)
    return "ldscripts/elf_i386_kot.xu";
  else if (bfd_link_relocatable (&link_info))
    return "ldscripts/elf_i386_kot.xr";
  else if (!config.text_read_only)
    return "ldscripts/elf_i386_kot.xbn";
  else if (!config.magic_demand_paged)
    return "ldscripts/elf_i386_kot.xn";
  else if (bfd_link_pie (&link_info)
	   && link_info.combreloc
	   && link_info.relro
	   && (link_info.flags & DF_BIND_NOW))
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xdwe";
      else
	return "ldscripts/elf_i386_kot.xdw";
    }
  else if (bfd_link_pie (&link_info)
	   && link_info.combreloc)
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xdce";
      else
	return "ldscripts/elf_i386_kot.xdc";
    }
  else if (bfd_link_pie (&link_info))
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xde";
      else
	return "ldscripts/elf_i386_kot.xd";
    }
  else if (bfd_link_dll (&link_info) && link_info.combreloc
	   && link_info.relro && (link_info.flags & DF_BIND_NOW))
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xswe";
      else
	return "ldscripts/elf_i386_kot.xsw";
    }
  else if (bfd_link_dll (&link_info) && link_info.combreloc)
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xsce";
      else
	return "ldscripts/elf_i386_kot.xsc";
    }
  else if (bfd_link_dll (&link_info))
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xse";
      else
	return "ldscripts/elf_i386_kot.xs";
    }
  else if (link_info.combreloc && link_info.relro
	   && (link_info.flags & DF_BIND_NOW))
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xwe";
      else
	return "ldscripts/elf_i386_kot.xw";
    }
  else if (link_info.combreloc)
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xce";
      else
	return "ldscripts/elf_i386_kot.xc";
    }
  else
    {
      if (link_info.separate_code)
	return "ldscripts/elf_i386_kot.xe";
      else
	return "ldscripts/elf_i386_kot.x";
    }
}

 
#define OPTION_LD_GENERATED_UNWIND_INFO	301
#define OPTION_NO_LD_GENERATED_UNWIND_INFO 302


enum elf_options
{
  OPTION_DISABLE_NEW_DTAGS = 400,
  OPTION_ENABLE_NEW_DTAGS,
  OPTION_GROUP,
  OPTION_EH_FRAME_HDR,
  OPTION_NO_EH_FRAME_HDR,
  OPTION_EXCLUDE_LIBS,
  OPTION_HASH_STYLE,
  OPTION_BUILD_ID,
  OPTION_AUDIT,
  OPTION_COMPRESS_DEBUG
};

static void
gldelf_i386_kot_add_options
  (int ns, char **shortopts, int nl, struct option **longopts,
   int nrl ATTRIBUTE_UNUSED, struct option **really_longopts ATTRIBUTE_UNUSED)
{
  static const char xtra_short[] = "z:P:";
  static const struct option xtra_long[] = {
    {"audit", required_argument, NULL, OPTION_AUDIT},
    {"Bgroup", no_argument, NULL, OPTION_GROUP},
    {"build-id", optional_argument, NULL, OPTION_BUILD_ID},
    {"compress-debug-sections", required_argument, NULL, OPTION_COMPRESS_DEBUG},
    {"depaudit", required_argument, NULL, 'P'},
    {"disable-new-dtags", no_argument, NULL, OPTION_DISABLE_NEW_DTAGS},
    {"enable-new-dtags", no_argument, NULL, OPTION_ENABLE_NEW_DTAGS},
    {"eh-frame-hdr", no_argument, NULL, OPTION_EH_FRAME_HDR},
    {"no-eh-frame-hdr", no_argument, NULL, OPTION_NO_EH_FRAME_HDR},
    {"exclude-libs", required_argument, NULL, OPTION_EXCLUDE_LIBS},
    {"hash-style", required_argument, NULL, OPTION_HASH_STYLE},
    
  {"ld-generated-unwind-info", no_argument, NULL,
   OPTION_LD_GENERATED_UNWIND_INFO},
  {"no-ld-generated-unwind-info", no_argument, NULL,
   OPTION_NO_LD_GENERATED_UNWIND_INFO},

    {NULL, no_argument, NULL, 0}
  };

  *shortopts = (char *) xrealloc (*shortopts, ns + sizeof (xtra_short));
  memcpy (*shortopts + ns, &xtra_short, sizeof (xtra_short));
  *longopts = (struct option *)
    xrealloc (*longopts, nl * sizeof (struct option) + sizeof (xtra_long));
  memcpy (*longopts + nl, &xtra_long, sizeof (xtra_long));
}

#define DEFAULT_BUILD_ID_STYLE	"sha1"

static bfd_boolean
gldelf_i386_kot_handle_option (int optc)
{
  switch (optc)
    {
    default:
      return FALSE;

    case OPTION_BUILD_ID:
      free ((char *) ldelf_emit_note_gnu_build_id);
      ldelf_emit_note_gnu_build_id = NULL;
      if (optarg == NULL)
	optarg = DEFAULT_BUILD_ID_STYLE;
      if (strcmp (optarg, "none"))
	ldelf_emit_note_gnu_build_id = xstrdup (optarg);
      break;

    case OPTION_COMPRESS_DEBUG:
      if (strcasecmp (optarg, "none") == 0)
	link_info.compress_debug = COMPRESS_DEBUG_NONE;
      else if (strcasecmp (optarg, "zlib") == 0)
	link_info.compress_debug = COMPRESS_DEBUG_GABI_ZLIB;
      else if (strcasecmp (optarg, "zlib-gnu") == 0)
	link_info.compress_debug = COMPRESS_DEBUG_GNU_ZLIB;
      else if (strcasecmp (optarg, "zlib-gabi") == 0)
	link_info.compress_debug = COMPRESS_DEBUG_GABI_ZLIB;
      else
	einfo (_("%F%P: invalid --compress-debug-sections option: `%s'\n"),
	       optarg);
      break;
    case OPTION_AUDIT:
	ldelf_append_to_separated_string (&audit, optarg);
	break;

    case 'P':
	ldelf_append_to_separated_string (&depaudit, optarg);
	break;

    case OPTION_DISABLE_NEW_DTAGS:
      link_info.new_dtags = FALSE;
      break;

    case OPTION_ENABLE_NEW_DTAGS:
      link_info.new_dtags = TRUE;
      break;

    case OPTION_EH_FRAME_HDR:
      link_info.eh_frame_hdr_type = DWARF2_EH_HDR;
      break;

    case OPTION_NO_EH_FRAME_HDR:
      link_info.eh_frame_hdr_type = 0;
      break;

    case OPTION_GROUP:
      link_info.flags_1 |= (bfd_vma) DF_1_GROUP;
      /* Groups must be self-contained.  */
      link_info.unresolved_syms_in_objects = RM_DIAGNOSE;
      link_info.unresolved_syms_in_shared_libs = RM_DIAGNOSE;
      break;

    case OPTION_EXCLUDE_LIBS:
      add_excluded_libs (optarg);
      break;

    case OPTION_HASH_STYLE:
      link_info.emit_hash = FALSE;
      link_info.emit_gnu_hash = FALSE;
      if (strcmp (optarg, "sysv") == 0)
	link_info.emit_hash = TRUE;
      else if (strcmp (optarg, "gnu") == 0)
	link_info.emit_gnu_hash = TRUE;
      else if (strcmp (optarg, "both") == 0)
	{
	  link_info.emit_hash = TRUE;
	  link_info.emit_gnu_hash = TRUE;
	}
      else
	einfo (_("%F%P: invalid hash style `%s'\n"), optarg);
      break;

    case 'z':
      if (strcmp (optarg, "defs") == 0)
	link_info.unresolved_syms_in_objects = RM_DIAGNOSE;
      else if (strcmp (optarg, "undefs") == 0)
	link_info.unresolved_syms_in_objects = RM_IGNORE;
      else if (strcmp (optarg, "muldefs") == 0)
	link_info.allow_multiple_definition = TRUE;
      else if (CONST_STRNEQ (optarg, "max-page-size="))
	{
	  char *end;

	  config.maxpagesize = strtoul (optarg + 14, &end, 0);
	  if (*end || (config.maxpagesize & (config.maxpagesize - 1)) != 0)
	    einfo (_("%F%P: invalid maximum page size `%s'\n"),
		   optarg + 14);
	}
      else if (CONST_STRNEQ (optarg, "common-page-size="))
	{
	  char *end;
	  config.commonpagesize = strtoul (optarg + 17, &end, 0);
	  if (*end
	      || (config.commonpagesize & (config.commonpagesize - 1)) != 0)
	    einfo (_("%F%P: invalid common page size `%s'\n"),
		   optarg + 17);
	}
      else if (CONST_STRNEQ (optarg, "stack-size="))
	{
	  char *end;
	  link_info.stacksize = strtoul (optarg + 11, &end, 0);
	  if (*end || link_info.stacksize < 0)
	    einfo (_("%F%P: invalid stack size `%s'\n"), optarg + 11);
	  if (!link_info.stacksize)
	    /* Use -1 for explicit no-stack, because zero means
	       'default'.   */
	    link_info.stacksize = -1;
	}
      else if (strcmp (optarg, "execstack") == 0)
	{
	  link_info.execstack = TRUE;
	  link_info.noexecstack = FALSE;
	}
      else if (strcmp (optarg, "noexecstack") == 0)
	{
	  link_info.noexecstack = TRUE;
	  link_info.execstack = FALSE;
	}
      else if (strcmp (optarg, "globalaudit") == 0)
	{
	  link_info.flags_1 |= DF_1_GLOBAUDIT;
	}
      else if (CONST_STRNEQ (optarg, "start-stop-visibility="))
	{
	  if (strcmp (optarg, "start-stop-visibility=default") == 0)
	    link_info.start_stop_visibility = STV_DEFAULT;
	  else if (strcmp (optarg, "start-stop-visibility=internal") == 0)
	    link_info.start_stop_visibility = STV_INTERNAL;
	  else if (strcmp (optarg, "start-stop-visibility=hidden") == 0)
	    link_info.start_stop_visibility = STV_HIDDEN;
	  else if (strcmp (optarg, "start-stop-visibility=protected") == 0)
	    link_info.start_stop_visibility = STV_PROTECTED;
	  else
	    einfo (_("%F%P: invalid visibility in `-z %s'; "
		     "must be default, internal, hidden, or protected"),
		   optarg);
	}
      else if (strcmp (optarg, "global") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_GLOBAL;
      else if (strcmp (optarg, "initfirst") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_INITFIRST;
      else if (strcmp (optarg, "interpose") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_INTERPOSE;
      else if (strcmp (optarg, "loadfltr") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_LOADFLTR;
      else if (strcmp (optarg, "nodefaultlib") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_NODEFLIB;
      else if (strcmp (optarg, "nodelete") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_NODELETE;
      else if (strcmp (optarg, "nodlopen") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_NOOPEN;
      else if (strcmp (optarg, "nodump") == 0)
	link_info.flags_1 |= (bfd_vma) DF_1_NODUMP;
      else if (strcmp (optarg, "now") == 0)
	{
	  link_info.flags |= (bfd_vma) DF_BIND_NOW;
	  link_info.flags_1 |= (bfd_vma) DF_1_NOW;
	}
      else if (strcmp (optarg, "lazy") == 0)
	{
	  link_info.flags &= ~(bfd_vma) DF_BIND_NOW;
	  link_info.flags_1 &= ~(bfd_vma) DF_1_NOW;
	}
      else if (strcmp (optarg, "origin") == 0)
	{
	  link_info.flags |= (bfd_vma) DF_ORIGIN;
	  link_info.flags_1 |= (bfd_vma) DF_1_ORIGIN;
	}
      else if (strcmp (optarg, "combreloc") == 0)
	link_info.combreloc = TRUE;
      else if (strcmp (optarg, "nocombreloc") == 0)
	link_info.combreloc = FALSE;
      else if (strcmp (optarg, "nocopyreloc") == 0)
	link_info.nocopyreloc = TRUE;
      else if (strcmp (optarg, "relro") == 0)
	link_info.relro = TRUE;
      else if (strcmp (optarg, "norelro") == 0)
	link_info.relro = FALSE;
      else if (strcmp (optarg, "separate-code") == 0)
	link_info.separate_code = TRUE;
      else if (strcmp (optarg, "noseparate-code") == 0)
	link_info.separate_code = FALSE;
      else if (strcmp (optarg, "common") == 0)
	link_info.elf_stt_common = elf_stt_common;
      else if (strcmp (optarg, "nocommon") == 0)
	link_info.elf_stt_common = no_elf_stt_common;
      else if (strcmp (optarg, "text") == 0)
	link_info.textrel_check = textrel_check_error;
      else if (strcmp (optarg, "notext") == 0)
	link_info.textrel_check = textrel_check_none;
      else if (strcmp (optarg, "textoff") == 0)
	link_info.textrel_check = textrel_check_none;
  
      else if (strcmp (optarg, "noextern-protected-data") == 0)
	link_info.extern_protected_data = FALSE;
 
      else if (strcmp (optarg, "dynamic-undefined-weak") == 0)
	link_info.dynamic_undefined_weak = TRUE;
      else if (strcmp (optarg, "nodynamic-undefined-weak") == 0)
	link_info.dynamic_undefined_weak = FALSE;
 
      else if (strncmp (optarg, "call-nop=", 9) == 0)
	{
	  if (strcmp (optarg + 9, "prefix-addr") == 0)
	    {
	      params.call_nop_as_suffix = FALSE;
	      params.call_nop_byte = 0x67;
	    }
	  else if (strcmp (optarg + 9, "suffix-nop") == 0)
	    {
	      params.call_nop_as_suffix = TRUE;
	      params.call_nop_byte = 0x90;
	    }
	  else if (strncmp (optarg + 9, "prefix-", 7) == 0)
	    {
	      char *end;
	      params.call_nop_byte = strtoul (optarg + 16 , &end, 0);
	      if (*end)
		einfo (_("%F%P: invalid number for -z call-nop=prefix-: %s\n"),
		       optarg + 16);
	      params.call_nop_as_suffix = FALSE;
	    }
	  else if (strncmp (optarg + 9, "suffix-", 7) == 0)
	    {
	      char *end;
	      params.call_nop_byte = strtoul (optarg + 16, &end, 0);
	      if (*end)
		einfo (_("%F%P: invalid number for -z call-nop=suffix-: %s\n"),
		       optarg + 16);
	      params.call_nop_as_suffix = TRUE;
	    }
	  else
	    einfo (_("%F%P: unsupported option: -z %s\n"), optarg);
	}
 
      else if (strcmp (optarg, "ibtplt") == 0)
	params.ibtplt = TRUE;
      else if (strcmp (optarg, "ibt") == 0)
	params.ibt = TRUE;
      else if (strcmp (optarg, "shstk") == 0)
	params.shstk = TRUE;
      else if (strncmp (optarg, "cet-report=", 11) == 0)
	{
	  if (strcmp (optarg + 11, "none") == 0)
	    params.cet_report = cet_report_none;
	  else if (strcmp (optarg + 11, "warning") == 0)
	    params.cet_report = (cet_report_warning
				 | cet_report_ibt
				 | cet_report_shstk);
	  else if (strcmp (optarg + 11, "error") == 0)
	    params.cet_report = (cet_report_error
				 | cet_report_ibt
				 | cet_report_shstk);
	  else
	    einfo (_("%F%P: invalid option for -z cet-report=: %s\n"),
		   optarg + 11);
	}

      else
	einfo (_("%P: warning: -z %s ignored\n"), optarg);
      break;
 
    case OPTION_LD_GENERATED_UNWIND_INFO:
      link_info.no_ld_generated_unwind_info = FALSE;
      break;

    case OPTION_NO_LD_GENERATED_UNWIND_INFO:
      link_info.no_ld_generated_unwind_info = TRUE;
      break;

    case OPTION_DYNAMIC_LINKER:
      params.has_dynamic_linker = TRUE;
      return FALSE;

    case OPTION_NON_SHARED:
      /* Check if -static is passed at command-line before all input
	 files.  */
      if (!lang_has_input_file)
	params.static_before_all_inputs = TRUE;
      return FALSE;

    }

  return TRUE;
}


static void
gldelf_i386_kot_list_options (FILE * file)
{
  
  fprintf (file, _("\
  -z noextern-protected-data  Do not treat protected data symbol as external\n"));
 
  fprintf (file, _("\
  -z dynamic-undefined-weak   Make undefined weak symbols dynamic\n\
  -z nodynamic-undefined-weak Do not make undefined weak symbols dynamic\n"));
 
  fprintf (file, _("\
  -z call-nop=PADDING         Use PADDING as 1-byte NOP for branch\n"));
 
  fprintf (file, _("\
  -z ibtplt                   Generate IBT-enabled PLT entries\n"));
  fprintf (file, _("\
  -z ibt                      Generate GNU_PROPERTY_X86_FEATURE_1_IBT\n"));
  fprintf (file, _("\
  -z shstk                    Generate GNU_PROPERTY_X86_FEATURE_1_SHSTK\n"));
  fprintf (file, _("\
  -z cet-report=[none|warning|error] (default: none)\n\
                              Report missing IBT and SHSTK properties\n"));

}

struct ld_emulation_xfer_struct ld_elf_i386_kot_emulation =
{
  elf_x86_before_parse,
  syslib_default,
  hll_default,
  ldelf_after_parse,
  gldelf_i386_kot_after_open,
  after_check_relocs_default,
  ldelf_before_place_orphans,
  gldelf_i386_kot_after_allocation,
  set_output_arch_default,
  ldemul_default_target,
  gldelf_i386_kot_before_allocation,
  gldelf_i386_kot_get_script,
  "elf_i386_kot",
  "elf32-i386",
  finish_default,
  elf_x86_create_output_section_statements,
  ldelf_open_dynamic_archive,
  ldelf_place_orphan,
  NULL,
  NULL,
  gldelf_i386_kot_add_options,
  gldelf_i386_kot_handle_option,
  NULL,
  gldelf_i386_kot_list_options,
  ldelf_load_symbols,
  NULL,
  NULL,
  NULL,
  ldelf_emit_ctf_early,
  ldelf_examine_strtab_for_ctf,
  NULL
};
