package com.glumes.base.binder

import android.databinding.DataBindingUtil
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.glumes.codec.DecodeWrapper
import com.glumes.codec.EncodeWrapper
import com.glumes.databindingadapter.BindingViewHolder
import com.glumes.databindingadapter.ViewHolderBinder
import com.glumes.ffmpeginandroid.R
import com.glumes.ffmpeginandroid.databinding.AdapterBackgroundOperationBinding
import com.glumes.util.OPS_TYPE_DECODE
import com.glumes.util.OPS_TYPE_ENCODE

/**
 * Created by glumes on 17/04/2018
 */

data class BackOpsItem(var title: String, var opsType: Int, var ops: Int)


class BackOpsHandler {

    fun onClick(v: View, model: BackOpsItem) {

        when (model.opsType) {

            OPS_TYPE_DECODE -> {
                DecodeWrapper.handleDecode(model.ops)
            }

            OPS_TYPE_ENCODE -> {
                EncodeWrapper.handleEecode(model.ops)
            }
        }

    }
}


class BackOperationHolder(binding: AdapterBackgroundOperationBinding) : BindingViewHolder<BackOpsItem, AdapterBackgroundOperationBinding>(binding) {

    override fun onBind(data: BackOpsItem?, position: Int) {
        mBinding.model = data!!
        mBinding.handler = BackOpsHandler()
        mBinding.executePendingBindings()
    }
}

class BackOperationBinder : ViewHolderBinder<BackOpsItem, BackOperationHolder>() {

    override fun onBindViewHolder(holder: BackOperationHolder?, data: BackOpsItem?, position: Int) {
        holder!!.onBind(data, position)
    }

    override fun createViewHolder(inflater: LayoutInflater?, parent: ViewGroup?): BackOperationHolder {
        val mBinding = DataBindingUtil.inflate<AdapterBackgroundOperationBinding>(inflater!!, R.layout.adapter_background_operation, parent, false)
        return BackOperationHolder(mBinding)
    }
}











