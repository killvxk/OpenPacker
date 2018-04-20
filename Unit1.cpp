//---------------------------------------------------------------------------
// Programmed by AmesianX in powerhacker.net.
// This software was programmed by reference to the public simple-packer
// tutorial on the internet.

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "scControls"
#pragma link "scShellControls"
#pragma link "scModernControls"
#pragma link "SkinCtrls"
#pragma link "scStyleManager"
#pragma link "scStyledForm"
#pragma link "scAppPager"
#pragma link "scExtControls"
#pragma link "scGPVertPagers"
#pragma link "scOpenFileDialog"
#pragma link "scImageCollection"
#pragma link "scToolPager"
#pragma link "scAppPager"
#pragma link "scControls"
#pragma link "scExtControls"
#pragma link "scGPVertPagers"
#pragma link "scImageCollection"
#pragma link "scModernControls"
#pragma link "scOpenFileDialog"
#pragma link "scShellControls"
#pragma link "scStyledForm"
#pragma link "scStyleManager"
#pragma link "SkinCtrls"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	scMemo1->Clear();
	scComboBox1->ItemIndex = 8;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	TStyleManager::SetStyle("Glossy");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
	Form2->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Exit1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton3Click(TObject *Sender)
{
	scShellTreeView1->Path = "C:\\";
	scAppPager1->ActivePage = scAppPagerPage3;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton5Click(TObject *Sender)
{
	scAppPager1->ActivePage = scAppPagerPage1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scGPVertTabControl1ChangeTab(TObject *Sender)
{
	scPageViewer1->PageIndex = scGPVertTabControl1->TabIndex;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton2Click(TObject *Sender)
{
	if (scOpenDialog1->Execute())
	{
		UnicodeString filename = scOpenDialog1->FileName;
		filename = filename.LowerCase();

		if (ExtractFileExt(filename) == ".exe" || ExtractFileExt(filename) == ".dll")
		{
			scEdit1->Text = scOpenDialog1->FileName;

			// scEdit2->Text = StringReplace(filename, ".exe", "_packed.exe", Sysutils::TReplaceFlags() << Sysutils::rfReplaceAll);
			scEdit2->Text = ExtractFilePath(filename) + "packed_" + ExtractFileName(filename);

			scButton5->Down = true;
			scAppPager1->ItemIndex = 0;
			scGPVertTabControl1->TabIndex = 0;
		}
		else {
			ShowMessage("실행 파일(EXE/DLL)만 팩킹할 수 있습니다.");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scShellListView1DblClick(TObject *Sender)
{
	UnicodeString filename = scShellListView1->GetSelectedFile();
	filename = filename.LowerCase();

	if (ExtractFileExt(filename) == ".exe" || ExtractFileExt(filename) == ".dll")
	{
		if (!DirectoryExists(filename))
		{
			scEdit1->Text = scShellListView1->GetSelectedFile();

			// scEdit2->Text = StringReplace(filename, ".exe", "_packed.exe", Sysutils::TReplaceFlags() << Sysutils::rfReplaceAll);
			scEdit2->Text = ExtractFilePath(filename) + "packed_" + ExtractFileName(filename);

			scButton5->Down = true;
			scAppPager1->ItemIndex = 0;
			scGPVertTabControl1->TabIndex = 0;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton6Click(TObject *Sender)
{
	scButton5->Down = true;
	scAppPager1->ItemIndex = 0;
	scGPVertTabControl1->TabIndex = 2;
}
//---------------------------------------------------------------------------
int __fastcall TForm1::Packing(UnicodeString in_filename, int argc, char *argv[])
{
	scMemo1->Clear();

	scButton4->Enabled = false;
	TCursor Save_Cursor = Screen->Cursor;
	Screen->Cursor = crHourGlass;

	namespace po = boost::program_options;

	boost::timer pack_timer;

	bool force_mode = false;
	bool repack_resources;
	bool rebuild_load_config;
	bool strip_dos_headers;
	unsigned long file_alignment;

	std::string input_file_name;
	std::string output_file_name;

	po::options_description visible_options("Open Packer ver 1.0\nCommand Line Options");

	try
	{
		po::options_description cmdline;

		visible_options.add_options()
			("out-file,o", po::value<std::string>(&output_file_name), "Output file name")
			("file-align,a", po::value<unsigned long>(&file_alignment)->default_value(512), "Packed file alignment")
			("strip-dos,s", po::value<bool>(&strip_dos_headers)->default_value(true), "Strip DOS headers")
			("repack-res,r", po::value<bool>(&repack_resources)->default_value(true), "Repack resources")
			("build-load-config,l", po::value<bool>(&rebuild_load_config)->default_value(true), "Rebuild Load Config directory")
			("force,f", "Force packing of possibly incorrect binaries");

		cmdline.add(visible_options);
		cmdline.add_options()
			("image", po::value<std::string>(&input_file_name), "PE image to pack");

		po::positional_options_description desc_pos;
		desc_pos.add("image", 1);

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(cmdline).positional(desc_pos).run(), vm);
		po::notify(vm);
		/*
		if(vm.count("force"))
		{
			scMemo1->Lines->Add(L"Force mode is active!");
			force_mode = true;
		}
		*/
	}
	catch(const std::exception& e)
	{
		scMemo1->Lines->Add(UnicodeString(e.what()));
		return 0;
	}

	input_file_name = AnsiString(in_filename).c_str();
	if(in_filename.IsEmpty()) {
		Screen->Cursor = Save_Cursor;
		scButton4->Enabled = true;
		return -1;
	}

	scMemo1->Lines->Add(L"Packing " + in_filename);

	std::auto_ptr<std::ifstream> file;
	file.reset(new std::ifstream(AnsiString(in_filename).c_str(), std::ios::in | std::ios::binary));
	if(!*file)
	{
		scMemo1->Lines->Add(L"Cannot open " + in_filename);
		Screen->Cursor = Save_Cursor;
		scButton4->Enabled = true;
		return -1;
	}

	try
	{
		pe_base image(*file, pe_properties_32(), false);
		file.reset(0);

		if(image.is_dotnet()) // && !force_mode)
		{
			scMemo1->Lines->Add(L".NET 이미지는 팩킹할 수 없습니다.");
			Screen->Cursor = Save_Cursor;
			scButton4->Enabled = true;
			return -1;
		}

		{
			scMemo1->Lines->Add(L"Entropy of sections: ");
			double entropy = entropy_calculator::calculate_entropy(image);
			scMemo1->Lines->Add(entropy);
			if(entropy > 6.8)
			{
				ShowMessage(L"파일이 이미 팩킹되어 있는 것 같습니다. 확인해주십시오.");
				/*
				Screen->Cursor = Save_Cursor;
				scButton4->Enabled = true;
				if(!force_mode)
					return -1;
				*/
			}
		}

		if(lzo_init() != LZO_E_OK)
		{
			scMemo1->Lines->Add(L"Error initializing..");
			Screen->Cursor = Save_Cursor;
			scButton4->Enabled = true;
			return -1;
		}

		scMemo1->Lines->Add(L"Reading sections...");

		const section_list& sections = image.get_image_sections();
		if(sections.empty())
		{
			scMemo1->Lines->Add(L"File has no sections!");
			Screen->Cursor = Save_Cursor;
			scButton4->Enabled = true;
			return -1;
		}

		packed_file_info basic_info = {0};
		basic_info.number_of_sections = sections.size();
		basic_info.lock_opcode = 0xf0;

		basic_info.original_import_directory_rva = image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_IMPORT);
		basic_info.original_import_directory_size = image.get_directory_size(IMAGE_DIRECTORY_ENTRY_IMPORT);
		basic_info.original_resource_directory_rva = image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_RESOURCE);
		basic_info.original_resource_directory_size = image.get_directory_size(IMAGE_DIRECTORY_ENTRY_RESOURCE);
		basic_info.original_relocation_directory_rva = image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_BASERELOC);
		basic_info.original_relocation_directory_size = image.get_directory_size(IMAGE_DIRECTORY_ENTRY_BASERELOC);
		basic_info.original_load_config_directory_rva = image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
		basic_info.original_entry_point = image.get_ep();
		basic_info.total_virtual_size_of_sections = image.get_size_of_image();

		std::string packed_sections_info;
		{
			packed_sections_info.resize(sections.size() * sizeof(packed_section));
			std::string raw_section_data;
			unsigned long current_section = 0;

			for(section_list::const_iterator it = sections.begin(); it != sections.end(); ++it, ++current_section)
			{
				const section& s = *it;
				{
					packed_section& info
						= reinterpret_cast<packed_section&>(packed_sections_info[current_section * sizeof(packed_section)]);

					info.characteristics = s.get_characteristics();
					info.pointer_to_raw_data = s.get_pointer_to_raw_data();
					info.size_of_raw_data = s.get_size_of_raw_data();
					info.virtual_address = s.get_virtual_address();
					info.virtual_size = s.get_virtual_size();
					memset(info.name, 0, sizeof(info.name));
					memcpy(info.name, s.get_name().c_str(), s.get_name().length());
				}

				if(s.get_raw_data().empty())
					continue;

				raw_section_data += s.get_raw_data();
			}

			if(raw_section_data.empty())
			{
				scMemo1->Lines->Add(L"All sections of PE file are empty!");
				Screen->Cursor = Save_Cursor;
				scButton4->Enabled = true;
				return -1;
			}

			packed_sections_info += raw_section_data;
		}

		section new_section;
		new_section.set_name(".rsrc");
		new_section.readable(true).writeable(true).executable(true);
		std::string& out_buf = new_section.get_raw_data();
		boost::scoped_array<lzo_align_t> work_memory(new lzo_align_t[LZO1Z_999_MEM_COMPRESS]);
		lzo_uint src_length = packed_sections_info.size();
		basic_info.size_of_unpacked_data = src_length;
		lzo_uint out_length = 0;
		out_buf.resize(src_length + src_length / 16 + 64 + 3);
		scMemo1->Lines->Add(L"Packing data...");

		UnicodeString nLevel = scComboBox1->Items->Strings[scComboBox1->ItemIndex];
		// 사실은 구라.. 그냥 LZO 압축레벨인데.. 중소기업청에 창업 투자금을
		// 신청할 때 이런부류의 소프트웨어에는 반드시 난독화 레벨이 존재해야
		// 점수를 받을 수 있고 난독화 난이도별 성능 평가표를 만들어야 좋은
		// 평가를 받을 수 있다는 컨설팅 도움을 받아 어거지로 구겨넣은 내용이다.
		// (더러워도 슬퍼도 나는 안운다.. 세상은 다 그런거다..)
		scMemo1->Lines->Add(L"난독화 " + nLevel + L" 레벨로 팩킹됩니다.");

		if (LZO_E_OK != lzo1z_999_compress_level(reinterpret_cast<const unsigned char*>(packed_sections_info.data()),
												 src_length,
												 reinterpret_cast<unsigned char*>(&out_buf[0]),
												 &out_length,
												 work_memory.get(),
												 NULL,
												 0,
												 (lzo_callback_p)0,
												 StrToInt(nLevel)))
		{
			scMemo1->Lines->Add(L"Error compressing data!");
			Screen->Cursor = Save_Cursor;
			scButton4->Enabled = true;
			return -1;
		}

		scMemo1->Lines->Add(L"Packing complete...");
		basic_info.size_of_packed_data = out_length;
		out_buf.resize(out_length);
		out_buf = std::string(reinterpret_cast<const char*>(&basic_info), sizeof(basic_info)) + out_buf;
		if(out_buf.size() >= src_length)
		{
			scMemo1->Lines->Add(L"File is incompressible!");
			/*
			Screen->Cursor = Save_Cursor;
			scButton4->Enabled = true;
			if(!force_mode)
				return -1;
			*/
		}

		std::auto_ptr<tls_info> tls;
		if(image.has_tls())
		{
			scMemo1->Lines->Add(L"Reading TLS...");
			tls.reset(new tls_info(get_tls_info(image)));
		}

		exported_functions_list exports;
		export_info exports_info;
		if(image.has_exports())
		{
			scMemo1->Lines->Add(L"Reading exports...");
			exports = get_exported_functions(image, exports_info);
		}

		std::auto_ptr<image_config_info> load_config;
		if(image.has_config() && rebuild_load_config)
		{
			scMemo1->Lines->Add(L"Reading Image Load Config...");

			try
			{
				load_config.reset(new image_config_info(get_image_config(image)));
			}
			catch(const pe_exception& e)
			{
				image.remove_directory(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
				scMemo1->Lines->Add(L"Error reading load config directory: " + UnicodeString(e.what()));
			}
		}
		else
		{
			image.remove_directory(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG);
		}

		{
			const section& first_section = image.get_image_sections().front();
			new_section.set_virtual_address(first_section.get_virtual_address());
			const section& last_section = image.get_image_sections().back();
			DWORD total_virtual_size =
				last_section.get_virtual_address()
				+ pe_utils::align_up(last_section.get_virtual_size(), image.get_section_alignment())
				- first_section.get_virtual_address();

			resource_directory new_root_dir;

			if(image.has_resources() && repack_resources)
			{
				scMemo1->Lines->Add(L"Repacking resources...");
				resource_directory root_dir = get_resources(image);
				pe_resource_viewer res(root_dir);
				pe_resource_manager new_res(new_root_dir);

				try
				{
					pe_resource_viewer::resource_id_list icon_id_list(res.list_resource_ids(pe_resource_viewer::resource_icon_group));
					pe_resource_viewer::resource_name_list icon_name_list(res.list_resource_names(pe_resource_viewer::resource_icon_group));
					if(!icon_name_list.empty())
					{
						resource_cursor_icon_writer(new_res).add_icon(
							resource_cursor_icon_reader(res).get_icon_by_name(icon_name_list[0]),
							icon_name_list[0],
							res.list_resource_languages(pe_resource_viewer::resource_icon_group, icon_name_list[0]).at(0));
					}
					else if(!icon_id_list.empty())
					{
						resource_cursor_icon_writer(new_res).add_icon(
							resource_cursor_icon_reader(res).get_icon_by_id(icon_id_list[0]),
							icon_id_list[0],
							res.list_resource_languages(pe_resource_viewer::resource_icon_group, icon_id_list[0]).at(0));
					}
				}
				catch(const pe_exception&)
				{
				}

				try
				{
					pe_resource_viewer::resource_id_list manifest_id_list(res.list_resource_ids(pe_resource_viewer::resource_manifest));
					if(!manifest_id_list.empty())
					{
						new_res.add_resource(
							res.get_resource_data_by_id(pe_resource_viewer::resource_manifest, manifest_id_list[0]).get_data(),
							pe_resource_viewer::resource_manifest,
							manifest_id_list[0],
							res.list_resource_languages(pe_resource_viewer::resource_manifest, manifest_id_list[0]).at(0));
					}
				}
				catch(const pe_exception&)
				{
				}

				try
				{
					pe_resource_viewer::resource_id_list version_info_id_list(res.list_resource_ids(pe_resource_viewer::resource_version));
					if(!version_info_id_list.empty())
					{
						new_res.add_resource(
							res.get_resource_data_by_id(pe_resource_viewer::resource_version, version_info_id_list[0]).get_data(),
							pe_resource_viewer::resource_version,
							version_info_id_list[0],
							res.list_resource_languages(pe_resource_viewer::resource_version, version_info_id_list[0]).at(0));
					}
				}
				catch(const pe_exception&)
				{
				}
			}

			image.get_image_sections().clear();
			image.realign_file(file_alignment);
			section& added_section = image.add_section(new_section);
			image.set_section_virtual_size(added_section, total_virtual_size);

			scMemo1->Lines->Add(L"Creating imports...");

			import_library mod_kernel32;
			mod_kernel32.set_name("kernel32.dll");

			imported_function func;
			func.set_name("LoadLibraryA");
			mod_kernel32.add_import(func);

			func.set_name("GetProcAddress");
			mod_kernel32.add_import(func);

			DWORD load_library_address_rva = pe_base::rva_from_section_offset(added_section,
				offsetof(packed_file_info, load_library_a));

			mod_kernel32.set_rva_to_iat(load_library_address_rva);

			imported_functions_list imports;
			imports.push_back(mod_kernel32);
			import_rebuilder_settings settings;

			settings.build_original_iat(false);
			settings.save_iat_and_original_iat_rvas(true, true);
			settings.set_offset_from_section_start(added_section.get_raw_data().size());

			if(!new_root_dir.get_entry_list().empty())
				settings.enable_auto_strip_last_section(false);

			rebuild_imports(image, imports, added_section, settings);

			if(!new_root_dir.get_entry_list().empty())
				rebuild_resources(image, new_root_dir, added_section, added_section.get_raw_data().size());

			if(tls.get())
			{
				packed_file_info* info = reinterpret_cast<packed_file_info*>(&added_section.get_raw_data()[0]);
				info->original_tls_index_rva = tls->get_index_rva();
				if(!tls->get_tls_callbacks().empty())
					info->original_rva_of_tls_callbacks = tls->get_callbacks_rva();
				tls->set_index_rva(pe_base::rva_from_section_offset(added_section, offsetof(packed_file_info, tls_index)));
			}
		}

		DWORD first_callback_offset = 0;
		{
			section unpacker_section;
			unpacker_section.set_name("openpack");
			unpacker_section.readable(true).executable(true).writeable(true);
			{
				scMemo1->Lines->Add(L"Writing unpacker stub, size = " + UnicodeString(sizeof(unpacker_data)) + " bytes");

				std::string& unpacker_section_data = unpacker_section.get_raw_data();
				unpacker_section_data = std::string(reinterpret_cast<const char*>(unpacker_data), sizeof(unpacker_data));
				*reinterpret_cast<DWORD*>(&unpacker_section_data[original_image_base_offset]) = image.get_image_base_32();
				*reinterpret_cast<DWORD*>(&unpacker_section_data[original_image_base_no_fixup_offset]) = image.get_image_base_32();
				*reinterpret_cast<DWORD*>(&unpacker_section_data[rva_of_first_section_offset]) = image.get_image_sections().at(0).get_virtual_address();
			}

			section& unpacker_added_section = image.add_section(unpacker_section);

			if(tls.get() || image.has_exports() || image.has_reloc() || load_config.get())
			{
				unpacker_added_section.get_raw_data().resize(sizeof(unpacker_data));
			}

			if(tls.get())
			{
				scMemo1->Lines->Add(L"Rebuilding TLS...");

				std::string& data = unpacker_added_section.get_raw_data();

				DWORD directory_pos = data.size();
				data.resize(data.size() + sizeof(IMAGE_TLS_DIRECTORY32) + sizeof(DWORD));

				if(!tls->get_tls_callbacks().empty())
				{
					first_callback_offset = data.size();
					data.resize(data.size() + sizeof(DWORD) * (tls->get_tls_callbacks().size() + 1));
					*reinterpret_cast<DWORD*>(&data[first_callback_offset]) =
						image.rva_to_va_32(pe_base::rva_from_section_offset(unpacker_added_section, empty_tls_callback_offset));
					tls->set_callbacks_rva(pe_base::rva_from_section_offset(unpacker_added_section, first_callback_offset));
					reinterpret_cast<packed_file_info*>(&image.get_image_sections().at(0).get_raw_data()[0])->new_rva_of_tls_callbacks = tls->get_callbacks_rva();
				}
				else
				{
					tls->set_callbacks_rva(0);
				}

				tls->clear_tls_callbacks();
				tls->set_raw_data_start_rva(pe_base::rva_from_section_offset(unpacker_added_section, data.size()));
				tls->recalc_raw_data_end_rva();
				rebuild_tls(image, *tls, unpacker_added_section, directory_pos, false, false, tls_data_expand_raw, true, false);
				unpacker_added_section.get_raw_data() += tls->get_raw_data();
				image.set_section_virtual_size(unpacker_added_section, data.size() + tls->get_size_of_zero_fill());
				if(!image.has_reloc() && !image.has_exports() && !load_config.get())
					pe_utils::strip_nullbytes(unpacker_added_section.get_raw_data());
				image.prepare_section(unpacker_added_section);
			}
			image.set_ep(image.rva_from_section_offset(unpacker_added_section, 0));
		}

		if(load_config.get())
		{
			scMemo1->Lines->Add(L"Repacking load configuration...");

			section& unpacker_section = image.get_image_sections().at(1);
			load_config->clear_lock_prefix_list();
			load_config->add_lock_prefix_rva(pe_base::rva_from_section_offset(image.get_image_sections().at(0), offsetof(packed_file_info, lock_opcode)));
			rebuild_image_config(image, *load_config, unpacker_section, unpacker_section.get_raw_data().size(), true, true, true, !image.has_reloc() && !image.has_exports());
		}

		if(image.has_reloc())
		{
			scMemo1->Lines->Add(L"Creating relocations...");
			relocation_table_list reloc_tables;
			section& unpacker_section = image.get_image_sections().at(1);
			{
				relocation_table table;
				table.set_rva(unpacker_section.get_virtual_address());
				table.add_relocation(relocation_entry(original_image_base_offset, IMAGE_REL_BASED_HIGHLOW));
				reloc_tables.push_back(table);
			}

			if(tls.get())
			{
				DWORD tls_directory_offset = image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_TLS)
					- image.section_from_directory(IMAGE_DIRECTORY_ENTRY_TLS).get_virtual_address();
				relocation_table table;
				table.set_rva(image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_TLS));
				table.add_relocation(relocation_entry(static_cast<WORD>(
					offsetof(IMAGE_TLS_DIRECTORY32, StartAddressOfRawData)), IMAGE_REL_BASED_HIGHLOW));
				table.add_relocation(relocation_entry(static_cast<WORD>(
					offsetof(IMAGE_TLS_DIRECTORY32, EndAddressOfRawData)), IMAGE_REL_BASED_HIGHLOW));
				table.add_relocation(relocation_entry(static_cast<WORD>(
					offsetof(IMAGE_TLS_DIRECTORY32, AddressOfIndex)), IMAGE_REL_BASED_HIGHLOW));

				if(first_callback_offset)
				{
					table.add_relocation(relocation_entry(static_cast<WORD>(offsetof(IMAGE_TLS_DIRECTORY32, AddressOfCallBacks)), IMAGE_REL_BASED_HIGHLOW));
					table.add_relocation(relocation_entry(static_cast<WORD>(tls->get_callbacks_rva() - table.get_rva()), IMAGE_REL_BASED_HIGHLOW));
				}

				reloc_tables.push_back(table);
			}

			if(load_config.get())
			{
				DWORD config_directory_offset = image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG)
					- image.section_from_directory(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG).get_virtual_address();

				relocation_table table;
				table.set_rva(image.get_directory_rva(IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG));

				if(load_config->get_security_cookie_va())
					table.add_relocation(relocation_entry(static_cast<WORD>(offsetof(::IMAGE_LOAD_CONFIG_DIRECTORY32, SecurityCookie)), IMAGE_REL_BASED_HIGHLOW));

				if(load_config->get_se_handler_table_va())
					table.add_relocation(relocation_entry(static_cast<WORD>(offsetof(::IMAGE_LOAD_CONFIG_DIRECTORY32, SEHandlerTable)), IMAGE_REL_BASED_HIGHLOW));

				table.add_relocation(relocation_entry(static_cast<WORD>(offsetof(::IMAGE_LOAD_CONFIG_DIRECTORY32, LockPrefixTable)), IMAGE_REL_BASED_HIGHLOW));
				reloc_tables.push_back(table);
			}

			rebuild_relocations(image, reloc_tables, unpacker_section, unpacker_section.get_raw_data().size(), true, !image.has_exports());
		}

		if(image.has_exports())
		{
			scMemo1->Lines->Add(L"Repacking exports...");

			section& unpacker_section = image.get_image_sections().at(1);
			rebuild_exports(image, exports_info, exports, unpacker_section, unpacker_section.get_raw_data().size(), true);
		}

		image.remove_directory(IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT);
		image.remove_directory(IMAGE_DIRECTORY_ENTRY_IAT);
		image.remove_directory(IMAGE_DIRECTORY_ENTRY_SECURITY);
		image.remove_directory(IMAGE_DIRECTORY_ENTRY_DEBUG);
		image.strip_stub_overlay();

		std::string base_file_name;

		if(output_file_name.empty())
		{
			base_file_name = AnsiString(in_filename).c_str();
			std::string dir_name;
			std::string::size_type slash_pos;
			if((slash_pos = base_file_name.find_last_of("/\\")) != std::string::npos)
			{
				dir_name = base_file_name.substr(0, slash_pos + 1);
				base_file_name = base_file_name.substr(slash_pos + 1);
			}

			base_file_name = dir_name + "packed_" + base_file_name;
		}
		else
		{
			base_file_name = output_file_name;
		}

		std::ofstream new_pe_file(base_file_name.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
		if(!new_pe_file)
		{
			scMemo1->Lines->Add(L"Cannot create " + UnicodeString(base_file_name.c_str()));
			Screen->Cursor = Save_Cursor;
			scButton4->Enabled = true;

			ShowMessage(L"파일을 생성할 수 없습니다. 관리자 권한으로 실행하십시오.");

			return -1;
		}

		rebuild_pe(image, new_pe_file, strip_dos_headers, false);

		scMemo1->Lines->Add(L"Packed image was saved to " + UnicodeString(base_file_name.c_str()));
		scMemo1->Lines->Add(L"Resulting sections entropy: " + UnicodeString(entropy_calculator::calculate_entropy(image)));
		scMemo1->Lines->Add(L"Finished in " + UnicodeString(pack_timer.elapsed()) + " sec");
		scMemo1->Lines->Add(L"팩킹이 성공적으로 완료되었습니다.");
	}
	catch(const pe_exception& e)
	{
		scMemo1->Lines->Add(L"Error: " + UnicodeString(e.what()));
		Screen->Cursor = Save_Cursor;
		scButton4->Enabled = true;
		return -1;
	}

	Screen->Cursor = Save_Cursor;
	scButton4->Enabled = true;

	return 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton4Click(TObject *Sender)
{
	scButton5->Down = true;
	scAppPager1->ItemIndex = 0;
	scGPVertTabControl1->TabIndex = 0;

	if (scEdit1->Text.IsEmpty())
	{
		ShowMessage(L"팩킹할 파일을 선택한 후 프로텍트 시작을 해주십시오.");
		scEdit1->SetFocus();
	}
	else {
		char *argv[] = {
			AnsiString(scEdit1->Text).c_str(),
			AnsiString(scEdit1->Text).c_str(),
			0
		};

		Packing(scEdit1->Text, 2, argv);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scCheckBox1Click(TObject *Sender)
{
	if (scCheckBox1->Checked)
	{
		ShowMessage(L"프로세스 보호를 위해 OpenProtector.dll 을 활성화 합니다.\nOpenProtector.dll 을 팩킹된 실행 파일과 함께 배포하십시오.");
	}
	else {
		ShowMessage(L"프로세스 보호가 해제되어 OpenProtector.dll 이 비활성화 됩니다.");
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::scButton7Click(TObject *Sender)
{
	if (scCheckBox1->Checked)
	{
		scMemo1->Lines->Add(L"오픈프로텍터 OpenProtector.dll 파일이 복사되었습니다.");
		scMemo1->Lines->Add(L"=> " + ExtractFilePath(scEdit2->Text) + L"OpenProtector.dll");
		UnicodeString filename = ExtractFilePath(scEdit2->Text) + L"OpenProtector.dll";
		CopyFileW(L"OpenProtector.dll", filename.c_str(), false);
	}

	SHELLEXECUTEINFO shell;

	shell.cbSize = sizeof(SHELLEXECUTEINFO);
	shell.fMask = NULL;
	shell.hwnd = NULL;
	shell.lpVerb = NULL;
	shell.lpFile = scEdit2->Text.c_str();
	shell.lpParameters = NULL;
	shell.lpDirectory = ExtractFilePath(scEdit2->Text).c_str();
	shell.nShow = SW_SHOWNORMAL;
	shell.hInstApp = NULL;

	ShellExecuteEx(&shell);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::New1Click(TObject *Sender)
{
	scEdit1->Clear();
	scEdit2->Clear();
	scMemo1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Save1Click(TObject *Sender)
{
    // 컴포넌트 저장
}
