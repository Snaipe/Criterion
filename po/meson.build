# we don't need to run gettext if i18n is enabled, but we certainly
# don't want to run it when it is disabled.
if not do_i18n.disabled()
	gettext = find_program('gettext', required: false)

	if gettext.found()
		add_project_arguments('-DGETTEXT_PACKAGE="criterion"', language: 'c')
		i18n.gettext(meson.project_name(),
			args: '--directory=' + meson.source_root()
		)
	endif
endif
